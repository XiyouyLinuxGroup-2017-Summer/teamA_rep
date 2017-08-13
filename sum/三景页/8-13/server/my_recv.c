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

