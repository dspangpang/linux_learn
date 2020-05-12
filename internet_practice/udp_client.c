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
#include <string.h>
#include <arpa/inet.h>
int main(){
	/*创建一个套接字的类型*/
	int fd; 
	char send_buf[128] = "hllello";
	char recv_buf[100] = {0};
	int addr_len;
	int len;
	/*创建服务端的地址结构体  IPV4*/
	struct sockaddr_in server_addr;
	/*创建客户端的地址结构体  IPV4*/
	struct sockaddr_in local_addr;
	struct sockaddr_in server_text_addr;
	fd = socket(AF_INET,SOCK_DGRAM,0);
	
	/*判断套接字是否创建成功*/
	if(fd<0){
		perror("socket");
		exit(-1);
	}
	printf("%d\n",fd);
	char * server_ip = "192.168.202.129";
	
	unsigned int server_port = 66799;

	
	
	/*清空结构体方便赋值*/
	bzero(&server_addr,sizeof(server_addr));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);

	inet_pton(AF_INET,server_ip,&server_addr.sin_addr.s_addr);
	
	
	
	/*向服务端发送消息*/
	sendto(fd,send_buf,strlen(send_buf),0,(struct sockaddr *)&server_addr,sizeof(server_addr));
	len = recvfrom(fd,recv_buf,sizeof(recv_buf),0,(struct sockaddr *)&server_text_addr,&addr_len);
	
	if(len<0){
		perror("recvfrom");
		
	}
	
	if(len>0){
		printf("len= %d \n",len);
		printf("%s \n",recv_buf);
		printf("端口号为%d\n",ntohs(server_text_addr.sin_port));
	}
	sendto(fd,send_buf,strlen(send_buf),0,(struct sockaddr *)&server_text_addr,sizeof(server_text_addr));
	
	len = recvfrom(fd,recv_buf,sizeof(recv_buf),0,(struct sockaddr *)&server_text_addr,&addr_len);
	
	if(len<0){
		perror("recvfrom");
		
	}
	
	if(len>0){
		printf("len= %d \n",len);
		printf("%s \n",recv_buf);
		printf("端口号为%d\n",ntohs(server_text_addr.sin_port));
	}
	
	/*关闭创建的套接字*/
	close(fd);
	
	return 0;
}