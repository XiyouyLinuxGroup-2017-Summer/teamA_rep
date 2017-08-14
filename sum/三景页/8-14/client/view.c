#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include"view.h"
#include"logic.h"
#include"my_recv.h"

extern int sockfd;
void view_menu()
{
	char flag;
	
	do{
		fflush(stdin);
		system("clear");
		printf("\n\n\n");
		printf("\t\t\t1\t\t注册\n");
		printf("\t\t\t2\t\t登陆\n");
		printf("\t\t\t3\t\t注销\n");
		printf("请选择:");
		scanf("%c",&flag);
		getchar();
		fflush(stdin);
		switch(flag)
		{
			case '1':
				send(sockfd,"1",2,0);
				reg_account(sockfd);
				break;
			case '2':
				send(sockfd,"2",2,0);
				if(lading(sockfd)==0);
					break;
				//view_afterlading(sockfd);
				break;
			case '3':
				send(sockfd,"3",2,0);
				return;
			default:
				system("clear");
				printf("输入有误请重新输入\n");
		}

	}while(1);
}
void reg_account(int sockfd)
{
	char passwd[20];
	int t=0;
	account_t account;
	printf("请输入账号:");
	s_gets(account.name,15);
	do
	{	if(t!=0)
		{
			printf("两次密码不匹配，清重新输入\n");
			t++;	
		}
		printf("\n请输入密码:");
		s_gets(account.passwd,20);
		printf("\n请再次输入密码:");
		s_gets(passwd,20);
	}while(strcmp(account.passwd,passwd)!=0);
	char buf[200];
	sendaccount(sockfd,account,buf);
	puts(buf);
	sleep(3);
}
int lading(int sockfd)
{
	account_t account;
	printf("请输入账号:");
	s_gets(account.name,15);
	printf("\n请输入密码:");
	s_gets(account.passwd,20);
	char buf[200];
	sendaccount(sockfd,account,buf);
	if(strcmp(buf,"success")==0)
		printf("lading success\n");
	else
		printf("lading fail\n");
	sleep(3);
}
/*void view_afterlading(int sockfd)
{
	char flag;
	
	do{
		system("clear");
		printf("\n\n\n");
		printf("\t\t\t1\t\t好友列表\n");
		printf("\t\t\t2\t\t发送消息\n");
		printf("\t\t\t3\t\t注销\n");
		scanf("%c",&flag);
		getchar();
		switch(flag)
		{
			case '1':
				break;
			case '2':
				send(sockfd,"2",2,0);
				if(lading(sockfd)==0);
					break;
				view_afterlading(sockfd);
				break;
			case '3':
				send(sockfd,"3",2,0);
				return;
			default:
				system("clear");
				printf("输入有误请重新输入\n");
		}
	}while(1);
}*/
