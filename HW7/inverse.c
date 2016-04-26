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

int main(int argc, const char** argv){
	int istream = open(argv[1], O_RDONLY);
	if(istream < 0){
		printf("failed to open file!\n");
	}
	char buf[1024];
	Info result;
	while(1){
		int cnt;
		if((cnt = read(istream, &result, sizeof(result))) > 0){
			printf( "%s\n",result.Surname);
		}else if (cnt == 0) {
			// printf("e2!");
			break;
		}
		else {
			printf("error!");
			close(istream);
			exit(-1);
		}
	}
	close(istream);
	return 0;
}