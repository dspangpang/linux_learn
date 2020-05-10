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
#include <fcntl.h>
int main(int argc ,char* argv[]){
	if(argc < 3){
		printf("输入的参数太少！ \n");
		exit(-1);
		
	}
	
	/*创建一个套接字的类型*/
	int fd;
	
	/*创建一个文件描述符*/
    int fp;
	
	int temporary_addr_len ;
	
	char send_buf [516] = {0}; 
    char recv_buf [516] = {0};
	/*创建服务端的地址结构体  IPV4*/
	struct sockaddr_in server_addr;
	
	/*与服务器连接后使用临时端口进行通信*/
	struct sockaddr_in temporary_addr;
	
	/*判断是上传还是下载    0是下载  1是上传   */ 
	if(argv[1] == 0 ){
		fp = open(argv[2],O_WRONLY|O_CREAT,0777);
		if(fp<0){
			perror("open");
			exit(-1);
		}
	}
		
	
	
	
	
	fd = socket(AF_INET,SOCK_DGRAM,0);
	
	/*判断套接字是否创建成功*/
	if(fd<0){
		perror("socket");
		close(fp);
		exit(-1);
	}

	char * server_ip = "192.168.202.129";
	
	unsigned int server_port = 69;

	
	/*清空结构体方便赋值*/
	bzero(&server_addr,sizeof(server_addr));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);
	inet_pton(AF_INET,server_ip,&server_addr.sin_addr.s_addr);
	
	sprintf(send_buf,%c%c%s%c%s%c%,0,1,"filename",0,"octet",0);
	
	/*向服务端发送下载请求*/
	sendto(fd,send_buf,sizeof(send_buf),0,(struct sockaddr *)&server_addr,sizeof(server_addr));
	
	/*从服务器端接收数据*/
	while(1){
		bzero(&recv_buf,sizeof(recv_buf));
		len = recvfrom(fd,recv_buf,sizeof(recv_buf),0,(struct sockaddr *)&temporary_addr,&temporary_addr_len);
		if(recv_buf[2] == 5){
			printf("%s",recv_buf+4);
			break;
		}
		if(516 == len && recv_buf[2] == 3){
			write(fp,recv_buf+4,512);
			sendto(fd,recv_buf,4,0,(struct sockaddr *)&temporary_addr,sizeof(temporary_addr));
		}
		if(516 > len && recv_buf[2] == 3){
			break;
		}
	}
	write(fd,recv_buf+4,len);
	
	
	/*关闭创建的套接字*/
	close(fd);
	close(fp);
	return 0;
}