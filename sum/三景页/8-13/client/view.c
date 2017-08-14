#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include"view.h"
#include"logic.h"

extern int sockfd;
void view_menu()
{
	char flag;
	
	do{
		printf("\t1\t\t注册\n");
		printf("\t2\t\t登陆\n");
		printf("\t3\t\t注销\n");
		scanf("%c",&flag);
		getchar();
		switch(flag)
		{
			case '1':
				send(sockfd,"1",2,0);
				reg_account(sockfd);
				break;
			case '2':
				send(sockfd,"2",2,0);
				lading();
				break;
			case '3':
				return;
			default:
				system("clear");
				printf("输入有误请重新输入\n");
		}

	}
	while(1);
}
void reg_account(int sockfd)
{
	char passwd[20];
	int t=0;
	account_t account;
	printf("请输入账号:");
	fgets(account.name,15,stdin);
	do
	{	if(t!=0)
		{
			printf("两次密码不匹配，清重新输入\n");
			t++;	
		}
		printf("\n请输入密码:");
		fgets(account.passwd,20,stdin);
		printf("\n请再次输入密码:");
		fgets(passwd,20,stdin);
	}while(strcmp(account.passwd,passwd)!=0);
	
	sendaccount(sockfd,account);
}
int lading()
{
	;
}
