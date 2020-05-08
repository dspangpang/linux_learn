/*****************************************************************************
Copyright: 
File name: udp_server.c
Description: 使用UDP协议完成客户端和服务端的通话
Author: jzz
Version: 
Date: 
History: 
*****************************************************************************/
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
int main(){
	/*创建一个套接字的类型*/
	int fd;     
	char send_buf[128] = "hello";
	char recv_buf[128] = {0};
	/*接收 recvfrom 函数的返回值*/
	int len ;
	
	/*创建服务端的地址结构体  IPV4*/
	struct sockaddr_in client_addr;
	int addr_len = sizeof(client_addr);
	
	/*创建客户端的地址结构体  IPV4*/
	struct sockaddr_in local_addr;
	
	fd = socket(AF_INET,SOCK_DGRAM,0);
	
	/*判断套接字是否创建成功*/
	if(fd<0){
		perror("socket");
		exit(-1);
	}
	printf("%d\n",fd);
	char * local_ip = "192.168.202.129";
	char * server_ip = "192.168.202.129";
	unsigned int local_port = 9997;
	unsigned int client_port ;

	
	
    bzero(&local_addr,sizeof(local_addr));
	
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(local_port);
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//inet_pton(AF_INET,local_ip,&local_addr.sin_addr.s_addr);
	
	/*绑定此进程端口号*/
	bind(fd,(struct sockaddr *)&local_addr,sizeof(local_addr));
	
	/*接收消息*/
	len = recvfrom(fd,recv_buf,sizeof(recv_buf),0,(struct sockaddr *)&client_addr,&addr_len);
	if(len<0){
		perror("recvfrom");
		
	}
	
	if(len>0){
		printf("len= %d \n",len);
		printf("%s \n",recv_buf);
	}

	
	/*关闭创建的套接字*/
	close(fd);
	
	return 0;
}