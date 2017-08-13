#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include"my_recv.h"

void my_err(const char* err_string,int line)
{
	fprintf(stderr,"line:%d",line);
	perror(err_string);
	exit(1);
}

void myrecv(int clifd,char* buf,int len)
{
	int sum=0,n;
	while(sum!=len)
	{
		n=recv(clifd,buf+sum,len-sum,0);
		if(n<0)
			my_err("myrecv",__LINE__);
		sum+=n;
	}
}
