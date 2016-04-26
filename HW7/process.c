#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>//for open
#include <unistd.h>//for close
#include <stdint.h>
#include <errno.h>
#include <dirent.h>

#define N_PROCESS 10
#define MASK ((0x1 << N_PROCESS) - 1)

char TERMINATE[] = "terminate\n";

typedef struct Info{
	char Surname[16];
	char Firstname[16];
	char Department[2];
	int ID;
	int Age;
}Info;

typedef struct{
	enum{
		INFO,
		MSG
	} type;
	union{
		Info info;
		int msg;
	} buf;
}__Result;

const __Result end = {.type = MSG, .buf.msg = -1};

const char* getfield(char* line, int num){
	const char* tok;
	for(tok = strtok(line, ";");
		tok && *tok;
		tok = strtok(NULL, ";\n")){
		if(!--num) return tok;
	}
	return NULL;
}


struct Info parse_line(char* line){
	struct Info inv;
	
	char* id = strdup(line);
	char* surname = strdup(line);
	char* firstname = strdup(line);		
	char* department = strdup(line);
	char* age = strdup(line);
	
	//ID
	inv.ID = atoi(getfield(id, 1));
	//firstname
	sscanf(getfield(firstname, 2), "%s", inv.Firstname);
	//surname
	sscanf(getfield(surname, 3), "%s", inv.Surname);
	//department
	sscanf(getfield(department, 4), "%s", inv.Department);
	//age
	inv.Age = atoi(getfield(age, 5));
	
	free(id);
	free(surname);
	free(firstname);
	free(department);
	free(age);
	return inv;
}

int child(int ifd, int ofd){
	char buf[1024];
	int len = 0;
	while (1) {
        len += read(ifd, buf + len, 1);
        if (buf[len - 1] == '\n'){
            buf[len] = '\0';
            break;
        }
        if (len == sizeof(buf) - 1){
            perror("Buffer overflow!");
            exit(EXIT_FAILURE);
        }
    }
    if(strcmp(TERMINATE, buf) == 0){
    	write(ofd, &end, sizeof(end));
    	return -1;
    } else{
    	struct Info inv = parse_line(buf);
    	__Result result;
    	result.type = INFO;
    	result.buf.info = inv;
    	write(ofd, &result, sizeof(result));
    	return 0;
    }
}

int reduce(pid_t pids[], int ifds[], int ofd){
	static int terminated = MASK;
	__Result result;
	if((terminated & MASK) == 0){
		fprintf(stdout, "all terminated \n");
		exit(EXIT_SUCCESS);
	}
	for(int i = 0; i < N_PROCESS; ++i){
		if(terminated & (0x1 << i)){
			read(ifds[i], &result, sizeof(result));
			if(result.type == INFO){
				write(ofd, &result.buf.info, sizeof(result.buf.info));;
			} else{
				terminated &= ~(0x1 << i);
			}
		}
	}
	return 0;
}
int main(int argc, const char** argv)
{
	pid_t pids[N_PROCESS];
	int iPipes[N_PROCESS * 2];
	int oPipes[N_PROCESS * 2];

	//open csv file
	FILE* fstream = fopen(argv[1], "r");
	if(fstream == NULL){
		perror("failed to open input file!");
		exit(EXIT_FAILURE);
	}

	//output file descriptor
	int ofd;
	if((ofd = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0640)) == -1) {
    	perror("failed to open output binary file!");
    	exit(EXIT_FAILURE);
  	}
	
	//prepare child processes
	for(int i = 0; i < N_PROCESS; ++i){
		//create the pipes
		if(pipe(iPipes + i * 2) < 0){
			perror("failed to create iPipes!");
			exit(EXIT_FAILURE);
		}
		if(pipe(oPipes + i * 2) < 0){
			perror("failed to create oPipes!");
			exit(EXIT_FAILURE);
		}

		while((pids[i] = fork()) < 0){
			perror("failed to fork child process!");
			sleep(1);
		}

		if(pids[i] == 0){
			//child process
			close(iPipes[i * 2 + 1]);
			close(oPipes[i * 2 + 0]);
			while(child(iPipes[i * 2], oPipes[i * 2 + 1]) == 0); 
			exit(EXIT_SUCCESS);
		} else{
			//father process
			close(iPipes[i * 2 + 0]);
			close(oPipes[i * 2 + 1]);
		}
	}

	int reducePipe[2];
	if(pipe(reducePipe) < 0){
		perror("failed to create pipe for reducer!");
		exit(EXIT_FAILURE);
	}
	pid_t reducer;
	if((reducer = fork()) < 0){
		perror("failed to fork for reducer!");
		exit(EXIT_FAILURE);
	}

	if(reducer == 0){
		//reducer
		close(reducePipe[0]);
		int ifds[N_PROCESS];
		for(int i = 0; i < N_PROCESS; ++i){
			ifds[i] = oPipes[i * 2];
		}
		while(reduce(pids, ifds, ofd) == 0);
		exit(EXIT_SUCCESS);
	} else{
		close(reducePipe[1]);
	}

	char* buffer = NULL;
	ssize_t len = 0;
	size_t buflen = 0;
	int child = 0;
	while((len = getline(&buffer, &buflen, fstream)) > 0){
		write(iPipes[child * 2 + 1], buffer, len);
		child = (child + 1) % N_PROCESS;
	}
	fclose(fstream);

	for(int i = 0; i < N_PROCESS; ++i){
		write(iPipes[i * 2 + 1], TERMINATE, sizeof(TERMINATE));
	}

	int returnStatus;
	for(int i = 0; i < N_PROCESS; ++i){
		waitpid(pids[i], &returnStatus, 0);
	}

	waitpid(reducer, &returnStatus, 0);
	fclose(fstream);
	close(ofd);
	return 0;
}

