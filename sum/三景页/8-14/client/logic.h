#pragma once

typedef struct account_t
{
	char name[15];
	char passwd[20];
}account_t;

int con_service();
void sendaccount(int sockfd,account_t account,char* buf);
