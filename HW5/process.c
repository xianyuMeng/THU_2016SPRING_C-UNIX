#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>//for open
#include <unistd.h>//for close
typedef struct Info{
	char Surname[16];
	char Firstname[16];
	char Department[2];
	int ID;
	int Age;
}Info;

int countlines(char *filename){
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

const char* getfield(char* line, int num){
	const char* tok;
	for(tok = strtok(line, ";");
		tok && *tok;
		tok = strtok(NULL, ";\n")){
		if(!--num) return tok;
	}
	return NULL;
}
void printbincharpad(char c, FILE* ostream)
{
	/**********little endian
	for (int i = 7; i >= 0; --i){
		putchar((c & (1 << i)) ? '1' : '0');
	}
	*************************/

	for(int i = 0; i <= 7; ++i){
		//putchar((c & (1 << i)) ? '1' : '0');
		fprintf(ostream, "%c", (c & (1 << i)) ? '1' : '0');			
	}
	fprintf(ostream, "\n");
	//putchar('\n');
}
void printchar2bin(char* p, int cnt, FILE* ostream){
	for(size_t i = 0; i < cnt; ++i){
		printbincharpad(p[i], ostream);
	}
}

int main(int argc, const char* argv[])
{
	FILE* fstream = fopen(argv[1], "r");
	FILE* ostream = fopen(argv[2], "wb");
	char line[1024];
	char buf[1024];
	int lines = countlines(argv[1]);
	printf("the file contains %d lines!\n", lines);
	struct Info *ptr;
	ptr = malloc(sizeof(struct Info) * lines);
	memset(ptr, 0, sizeof(struct Info) * lines);
	size_t i = 0;
	
	while(fgets(line, 1024, fstream)){
		char* id = strdup(line);
		char* surname = strdup(line);
		char* firstname = strdup(line);
		char* department = strdup(line);
		char* age = strdup(line);

		ptr[i].ID = atoi(getfield(id, 1));
		sscanf(getfield(firstname, 2), "%s", ptr[i].Firstname);
		sscanf(getfield(surname, 3), "%s", ptr[i].Surname);
		sscanf(getfield(department, 4), "%s", ptr[i].Department);
		ptr[i].Age = atoi(getfield(age, 5));
		
		printf("the ID is %d\n", ptr[i].ID);
		printf("the name is %s\n", ptr[i].Firstname);
		printf("the surname is %s\n", ptr[i].Surname);
		printf("the age is %d\n", ptr[i].Age);
		printf("the ID is %u\n", htonl(ptr[i].ID));
		
		fprintf(ostream, "%u\n", htonl(ptr[i].ID));
		printchar2bin(ptr[i].Firstname, sizeof(ptr[i].Firstname), ostream);
		printchar2bin(ptr[i].Surname, sizeof(ptr[i].Surname), ostream);
		printchar2bin(ptr[i].Department, sizeof(ptr[i].Department), ostream);
		fprintf(ostream, "%u\n", htonl(ptr[i].Age));

		++i;
		free(id);
		free(surname);
		free(firstname);
		free(department);
		free(age);
	}
	fclose(fstream);
	fclose(ostream);
	return 0;
}

