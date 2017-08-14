#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include"my_recv.h"
#include"logic.h"

#define  IP1   "47.94.12.103"
#define  IP2   "182.254.227.246"

int con_service()
{
	int				conn_fd;
	struct sockaddr_in serv_addr;

	memset(&serv_addr,0,sizeof(struct sockaddr_in));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(4507);
	if(inet_aton(IP1,&serv_addr.sin_addr)==0)
	{
		printf("invalid server ip address\n");
		exit(1);
	}
	conn_fd=socket(AF_INET,SOCK_STREAM,0);
	if(conn_fd<0)
		my_err("socket",__LINE__);
	if(connect(conn_fd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr))<0)
		my_err("connect",__LINE__);
	else
	{
		printf("Connect process!\n");
		return conn_fd;
	}
	/*for(i=1;i<argc;i++)
	{
		if(strcmp("-p",argv[i])==0)
		{
			serv_port=atoi(argv[i+1]);
			if(serv_port<0||serv_port>65535)
			{
				printf("invalid serv_addr.sin_port\n");
				exit(1);
			}
			else
				serv_addr.sin_port=htons(serv_port);
			continue;
		}
		if(strcmp("-a",argv[i])==0)
		{
			if(inet_aton(argv[i+1],&serv_addr.sin_addr)==0)
			{
				printf("invalid server ip address\n");
				exit(1);
			}
			continue;
		}
	}
	if(serv_addr.sin_port==0||serv_addr.sin_addr.s_addr==0)
	{
		printf("Usage: [-p] [ser_addr.sin_port] [-a] [serv_address\n]");
		exit(1);
	}
	conn_fd=socket(AF_INET,SOCK_STREAM,0);
	if(conn_fd<0)
		my_err("socket",__LINE__);
	if(connect(conn_fd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr))<0)
		my_err("connect",__LINE__);
	else
	{
		printf("Connect process!\n");
		return conn_fd;
	}*/
}
//发送账户
void sendaccount(int sockfd,account_t account)
{
	char buf[200];
	int len;
	if(send(sockfd,(void*)&account,sizeof(account),0)<0)
		my_err("sendaccount",__LINE__);
	myrecv(sockfd,buf,sizeof(buf));
	puts(buf);
}
