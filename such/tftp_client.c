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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc ,char* argv[]){
	
	if(argc < 3){
		printf("输入的参数太少！ \n");
		exit(-1);
		
	}
	
	/*recvfrom 接收参数*/
	int len = 0;
	
	/*sendto_len 接收参数*/
	int sendto_len = 0;
	
	/*创建一个套接字的类型*/
	int fd;
	
	/*创建一个文件描述符*/
    int fp;
	
	
	
	char send_buf [1024] = {0}; 
    char recv_buf [1024] = {0};
	/*创建服务端的地址结构体  IPV4*/
	struct sockaddr_in server_addr;
	
	/*与服务器连接后使用临时端口进行通信*/
	struct sockaddr_in temporary_addr ;
	
	int temporary_addr_len  =  sizeof(temporary_addr);
	
	fd = socket(AF_INET,SOCK_DGRAM,0);
	
	/*判断套接字是否创建成功*/
	if(fd<0){
		perror("socket");
		close(fp);
		printf("1\n");
		exit(-1);
	}

	char * server_ip = "192.168.202.129";
	
	unsigned int server_port = 12345;

	
	/*清空结构体方便赋值*/
	bzero(&server_addr,sizeof(server_addr));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);
	
	inet_pton(AF_INET,server_ip,&server_addr.sin_addr.s_addr);
	
	
	
	if(*(argv[1]) == '0'){
		sprintf(send_buf,"%c%c%s%c%s%c",0,1,argv[2],0,"octet",0);
		/*向服务端发送下载请求*/
		for(int i=0;i<1000;i++);
		sendto(fd,send_buf,sizeof(send_buf),0,(struct sockaddr *)&server_addr,sizeof(server_addr));
		
		printf("server的端口号为%d\n",ntohs(server_addr.sin_port));
		fp = open(argv[2],O_WRONLY|O_CREAT,0777);
		if(fp<0){
			perror("open");
			exit(-1);
		}
		/*从服务器端接收数据*/
		while(1){
			bzero(recv_buf,sizeof(recv_buf));
			printf("1\n");
			len = recvfrom(fd,recv_buf,sizeof(recv_buf),0,(struct sockaddr *)&temporary_addr,&temporary_addr_len);
			printf("len = %d\n",len);
			if(len<0){
				perror("recvfrom");
				exit(-1);
				close(fd);
				close(fp);
			}
			printf("server的临时端口号为%d\n",ntohs(temporary_addr.sin_port));
			if(recv_buf[1] == 5){
				printf("%s",recv_buf+4);
				break;
			}
			if(516 == len && recv_buf[1] == 3){
				write(fp,recv_buf+4,512);
				
				sendto_len = sendto(fd,recv_buf,4,0,(struct sockaddr *)&temporary_addr,sizeof(temporary_addr));
				printf("sendto_len = %d\n",sendto_len);
				if(sendto_len<0){
				perror("sendto");
				close(fd);
				close(fp);
				exit(-1);
				}
			
				printf("server的端口号为%d,数据包编号为 %d\n",ntohs(temporary_addr.sin_port),*((short*)(recv_buf+2)));
			}
			if(516 > len && recv_buf[1] == 3){
				break;
			}
			
		}
		write(fp,recv_buf+4,len);
		printf("server的端口号为%d,数据包编号为 %d\n",ntohs(temporary_addr.sin_port),*((short*)(recv_buf+2)));
		sendto_len = sendto(fd,recv_buf,4,0,(struct sockaddr *)&temporary_addr,sizeof(temporary_addr));
	}
	
	/*关闭创建的套接字*/
	close(fd);
	close(fp);
	return 0;
}