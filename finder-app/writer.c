#include<stdio.h>
#include<syslog.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
int main(int argc, char *argv[]) {
	openlog(NULL,0,LOG_USER);
	if(argc != 3) {
		syslog(LOG_ERR,"Incorrect number of arguments passed");
		printf("Incorrect number of arguments passed\n");
		return 1;
	}
	int fd;
	fd = open(argv[1],O_WRONLY | O_CREAT, S_IWUSR | S_IROTH | S_IRGRP | S_IRUSR);
	if(fd == -1) {
		syslog(LOG_ERR,"Failed to create file %s",argv[1]);
		printf("Failed to create file %s\n",argv[1]);
		return 1;
	}
	int str_len = strlen(argv[2]);
	int write_bytes = write(fd,argv[2],str_len);
	if(write_bytes == -1) {
		syslog(LOG_ERR,"Could not write %s to file %s",argv[2],argv[1]);
		printf("Could not write %s to file %s\n",argv[2],argv[1]);
		return 1;
	} else {
		syslog(LOG_DEBUG,"Writing %s to %s",argv[2],argv[1]);
		printf("Writing %s to %s LOL\n",argv[2],argv[1]);
	}
	return 0;

}
