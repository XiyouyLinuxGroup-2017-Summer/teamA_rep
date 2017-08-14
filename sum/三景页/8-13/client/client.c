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
#include"view.h"
#include"logic.h"

#define INVALID_USERINFO		'n'
#define VALID_USERINFO			'y'
int sockfd;

int main(int argc,char** argv)
{
	sockfd=con_service();
	view_menu();
	
	close(sockfd);
	return 0;
}

