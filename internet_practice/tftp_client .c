/*****************************************************************************
Copyright: 
File name: udp_client.c
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
    
	char send_buf [256] = {0}; 

	/*创建服务端的地址结构体  IPV4*/
	struct sockaddr_in server_addr;
	
	fd = socket(AF_INET,SOCK_DGRAM,0);
	
	/*判断套接字是否创建成功*/
	if(fd<0){
		perror("socket");
		exit(-1);
	}
	printf("%d\n",fd);
	char * server_ip = "192.168.202.129";
	
	unsigned int server_port = 69;

	
	
	/*清空结构体方便赋值*/
	bzero(&server_addr,sizeof(server_addr));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);
	inet_pton(AF_INET,server_ip,&server_addr.sin_addr.s_addr);
	
	sprintf(send_buf,,0,1,"filename",0,"octet",0);
	
	/*向服务端发送消息*/
	sendto(fd,send_buf,sizeof(send_buf),0,(struct sockaddr *)&server_addr,sizeof(server_addr));
	
	
	/*关闭创建的套接字*/
	close(fd);
	
	return 0;
}