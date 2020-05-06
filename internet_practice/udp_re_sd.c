#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
int main(){
	int fd;
	char buf[] = "hello";
	fd = socket(AF_INET,SOCK_STREAM,0);
	if(fd<0){
		perror("");
		exit(-1);
	}
	printf("fd = %d\n",fd);
	
	close(fd);
	
	return 0;
}