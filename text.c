# include "stdio.h"
# include "stdlib.h"
# include "string.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

union x {
	short a;
	char s[2];
}x;

int main(int argc,char*argv[]){
	x.a = 15;
	char b[20] = {0};
	b[0] = '0';
	b[1] = '3';
	b[2] = x.s[0];
	b[3] = x.s[1];
	b[4] = 'c';
	printf("%c\n",b[0]);
	printf("%c\n",b[1]);
	printf("%d\n",*((short*)(b+2)));
	
	return 0;
}




