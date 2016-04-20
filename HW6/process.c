#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>//for open
#include <unistd.h>//for close
#include <stdint.h>
#include <errno.h>
#include <dirent.h>
typedef struct Info{
	char Surname[16];
	char Firstname[16];
	char Department[2];
	int ID;
	int Age;
}Info;

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
/*
int countlines(char* filename){
	// count the number of lines in the file called filename                                    
	FILE *fp = fopen(filename,"r");
	int ch=0;
	int lines=0;
	if (fp == NULL) return 0;
	lines++;
	while(!feof(fp)){
  		ch = fgetc(fp);
  		if(ch == '\n'){
			lines++;
		}
	}
	fclose(fp);
	return lines;
}

void printbincharpad(char c, FILE* ostream)
{
	for(int i = 0; i <= 7; ++i){
		fprintf(ostream, "%c", (c & (1 << i)) ? '1' : '0');			
	}
	fprintf(ostream, "\n");
}
void printchar2bin(char* p, int cnt, FILE* ostream){
	for(size_t i = 0; i < cnt; ++i){
		printbincharpad(p[i], ostream);
	}
}
*/
int main(int argc, const char** argv)
{

	//get current path
	char cwd[1024];
   	if (getcwd(cwd, sizeof(cwd)) != NULL)
    	fprintf(stdout, "Current working dir: %s\n", cwd);
   	else
    	perror("getcwd() error");
	
	//open csv file
	FILE* fstream = fopen(argv[1], "r");
	if(fstream == NULL){
		perror("failed to open input file!");
		exit(EXIT_FAILURE);
	}
	//output file descriptor
	int ofd;
	//create temporary directory 
	char template[] = "temp";
	char* tempdir = mkdtemp(template);
	if (tempdir == NULL){
		perror("failed to create ftemporary directory!");
		exit(EXIT_FAILURE);
	}

	//count lines of the input file	
	char line[1024];
	//int lines = countlines(argv[1]);
	//printf("the file contains %d lines!\n", lines);
	struct Info inv;
	pid_t pid;

	while(fgets(line, 1024, fstream)){
		while((pid = fork()) < 0){
			perror("failed to fork child process!");
			sleep(1);
		}
		if(pid == 0){
			//child process
			//open output file
			chdir(tempdir);
			//child process id
			pid_t ch_pid = getpid();
			int outfd;//output file descriptor
			char outfname[16];
			sprintf(outfname, "child%d", ch_pid);
			if((outfd = open(outfname, O_WRONLY|O_CREAT|O_TRUNC, 0640)) < 0){
				perror("failed to create a temporary file for child process!");
				fprintf(stderr, "process od = %d failed", ch_pid);
				exit(EXIT_FAILURE);
			}
			inv = parse_line(line);
			if(write(outfd, &inv, sizeof(struct Info)) != sizeof(struct Info)){
				fprintf(stderr, "process id = %d failed to write the file!", ch_pid);
			}
			close(outfd);
			exit(EXIT_SUCCESS);
		}
	}

	while((pid = waitpid(-1, NULL, 0)) != -1) {
    	if (errno == ECHILD) break;
    }

    // open output file
	if((ofd = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0640)) == -1) {
    	perror("failed to open output binary file!");
    	exit(EXIT_FAILURE);
  	}

	//open temporary directory
	DIR* dir;
	int fd;
	struct dirent* pDirent;
	if((dir = opendir(tempdir)) == NULL){
		perror("failed to open temp directory");
		exit(EXIT_FAILURE);
	}
	else{
		//change directory
		if(chdir(tempdir) == -1){
		perror("failed to change directory!");
		exit(EXIT_FAILURE);
		}
		/*
		To enumerate directories on Unix, you can use opendir, readdir, and closedir. 
		To remove you use rmdir() on an empty directory 
		(i.e. at the end of your function, after deleting the children) 
		and unlink() on a file. 
		*/
		while((pDirent = readdir(dir)) != NULL){
			if(pDirent->d_type == DT_REG){
				if ((fd = open(pDirent -> d_name, O_RDONLY)) < 0) {
          			fprintf(stderr, "failed to open tmp file %s, %s \n", pDirent -> d_name, strerror(errno));
        		} 
        		else if (read(fd, &inv, sizeof(struct Info)) != sizeof(struct Info)) {
          			fprintf(stderr, "failed to read tmp file %s \n", pDirent -> d_name);
        		} 
        		else if (write(ofd, &inv, sizeof(struct Info)) != sizeof(struct Info)) {
          			fprintf(stderr, "failed to write tmp file!");
        		}
        		if (unlink(pDirent -> d_name) != 0) {
          			fprintf(stderr, "failed to remove tmp file %s, %s\n", pDirent -> d_name, strerror(errno));
        		}	
			}
		}
	}
	//remove temp directory
	//we are now under the temp directory cd .. and then remove it
	chdir(cwd);
	if(rmdir(tempdir) < 0){
		fprintf(stderr, "failed to remove the empty temp directory %s \n", strerror(errno));
	}

	close(ofd);
	fclose(fstream);
	return 0;
}

