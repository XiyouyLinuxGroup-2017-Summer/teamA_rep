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

#define INVALID_USERINFO		'n'
#define VALID_USERINFO			'y'

int get_userinfo(char* buf,int len)
{
	int i;
	int c;
	if(buf==NULL)
		return -1;
	i=0;
	while(((c=getchar())!='\n')&&(c!=EOF)&&(i<len-2))
		buf[i++]=c;
	buf[i++]='\n';
	buf[i++]='\0';
	return 0;
}

void input_userinfo(int conn_fd,const char* string)
{
	do{
		printf("%s:",string);
		if(get_userinfo(input_buf,32)<0)
		{
			printf("error return from get_userinfo\n");
			exit(1);
		}
		if(send(conn_fd,input_buf,strlen(input_buf),0)<0)
		{	
			my_err("data is too long\n");
			exit(1);
		}

	}
}
