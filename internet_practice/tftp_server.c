/*****************************************************************************
Copyright: 
File name: udp_client.c
Description: 使用tftp协议完成客户端和服务端的通话
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


union code{
	short data ;
	char  str[2];
};


int main(int argc ,char* argv[]){
	
	union code code_num;
	/*创建一个套接字的类型*/
	int fd;
	
	/*创建一个文件描述符*/
    int fp;
	
	int fd_tem;
	/*recvfrom 的接收回填参数*/
	
	int temporary_addr_len ;
	
	
	
	/*recvfrom 接收参数*/
	int len;
	
	/*read接收参数*/
	int read_len;
	 
	int sendto_len;
	
	char send_buf [1024] = {0}; 
    char recv_buf [1024] = {0};
	/*创建服务端的地址结构体  IPV4*/
	struct sockaddr_in server_addr;
	
	struct sockaddr_in client_addr ;
	
	int client_len = sizeof(client_addr);
	/*与服务器连接后使用临时端口进行通信*/
	struct sockaddr_in temporary_addr;
	unsigned int server_port = 12345;

	
	fd = socket(AF_INET,SOCK_DGRAM,0);
	
	
	/*判断套接字是否创建成功*/
	if(fd<0){
		perror("socket");
		exit(-1);
	}
	
	/*创建一个新的套接字作为临时端口*/
	fd_tem = socket(AF_INET,SOCK_DGRAM,0);
	
	if(fd_tem<0){
		perror("socket");
		close(fd);
		close(fp);
		exit(-1);
	}
	
	
    bzero(&server_addr,sizeof(server_addr));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);
	
	/*可以监听任何 ip 的 69号端口*/
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	
	
	
	
	//inet_pton(AF_INET,server_ip,&server_addr.sin_addr.s_addr);
	
	/*绑定第一个套接字的69号端口*/
	bind(fd,(struct sockaddr *)&server_addr,sizeof(server_addr));
	
	bzero(recv_buf,sizeof(recv_buf));
	
	len = recvfrom(fd,recv_buf,sizeof(recv_buf),0,(struct sockaddr *)&client_addr,&client_len);
	printf("客户端的端口号为%d\n",ntohs(client_addr.sin_port));
	
	if(len<0){
		perror("recvfrom");
		close(fd);
		exit(-1);
		
	}

	
	/*判断客户端要执行的操作*/
	if(recv_buf[1] == 1 ){
		/*下载请求执行过程*/
		fp = open(recv_buf+2,O_RDONLY);
		if(fp < 0){
			perror("open");
			close(fd);
			exit(-1);
		}
		code_num.data = 0;
		
		
		while(1){
			bzero(send_buf,sizeof(send_buf));
			bzero(recv_buf,sizeof(recv_buf));
			code_num.data++;
			read_len = read(fp,send_buf+4,512);
			if(read_len<0){
				perror("read");
				close(fp);
				close(fd);
				close(fd_tem);
				exit(-1);
			}
			send_buf[0] = 0;
			send_buf[1] = 3;
			send_buf[2] = code_num.str[0];
			send_buf[3] = code_num.str[1];
			sendto_len = sendto(fd_tem,send_buf,read_len+4,0,(struct sockaddr *)&client_addr,sizeof(client_addr));
			if(sendto_len<0){
				perror("sendto");
				close(fd);
				close(fp);
				close(fd_tem);
				exit(-1);
			}
			
			printf("客户端的端口号为%d,数据包编号为 %d\n",ntohs(client_addr.sin_port),*((short*)(send_buf+2)));
			bzero(&client_addr,sizeof(client_addr));
			len = recvfrom(fd_tem,recv_buf,sizeof(recv_buf),0,(struct sockaddr *)&client_addr,&client_len);
			if(len<0){
				perror("recv_buf");
				close(fd);
				close(fd_tem);
				close(fp);
				exit(-1);
			}
			printf("操作码为%d\n",recv_buf[1]);
			if(recv_buf[1] == 5){
				printf("%s",recv_buf+4);
				break;
			}
			if(*((short*)(recv_buf+2))!= *((short*)(send_buf+2))){
				printf("数据传输中出错，发生丢包\n");
				break;
			}
			if(sendto_len < 512){
				printf("文件下载完毕\n");
				break;
			}
		}
	}
	
	if(recv_buf[1] == 2){
		fp = open(recv_buf+2,O_WRONLY|O_CREAT,0777);
		if(fp<0){
			perror("open");
			exit(-1);
		}
		/*发送空应答*/
		bzero(send_buf,sizeof(send_buf));
		send_buf[0] = 0;
		send_buf[1] = 4;
		send_buf[2] = code_num.str[0];
		send_buf[3] = code_num.str[1];
		
		sendto_len = sendto(fd_tem,send_buf,4,0,(struct sockaddr *)&client_addr,sizeof(client_addr));
		
		if(sendto_len<0){
			perror("sendto");
			close(fd);
			close(fp);
			close(fd_tem);
			exit(-1);
		}
		
		/*从客户端接收数据*/
		while(1){
			bzero(recv_buf,sizeof(recv_buf));
			
			len = recvfrom(fd_tem,recv_buf,sizeof(recv_buf),0,(struct sockaddr *)&client_addr,&client_len);
			
			if(len<0){
				perror("recvfrom");
				close(fd);
				close(fd_tem);
				close(fp);
				exit(-1);
			}
			printf("client端口号为%d\n",ntohs(client_addr.sin_port));
			if(recv_buf[1] == 5){
				printf("%s",recv_buf+4);
				break;
			}
			if(516 == len && recv_buf[1] == 3){
				write(fp,recv_buf+4,512);
				sendto_len = sendto(fd_tem,recv_buf,4,0,(struct sockaddr *)&client_addr,sizeof(client_addr));
				printf("sendto_len = %d\n",sendto_len);
				if(sendto_len<0){
				perror("sendto");
				close(fd);
				close(fp);
				exit(-1);
				}
				printf("client的端口号为%d,数据包编号为 %d\n",ntohs(client_addr.sin_port),*((short*)(recv_buf+2)));
			}
			if(516 > len && recv_buf[1] == 3){
				break;
			}
			
		}
		write(fp,recv_buf+4,len);
		printf("client的端口号为%d,数据包编号为 %d\n",ntohs(client_addr.sin_port),*((short*)(recv_buf+2)));
		sendto_len = sendto(fd_tem,recv_buf,4,0,(struct sockaddr *)&client_addr,sizeof(client_addr));
	}

	
	
	/*关闭创建的套接字*/
	close(fd);
	close(fd_tem);
	close(fp);
	
	return 0;
}