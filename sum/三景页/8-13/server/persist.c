#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<stdlib.h>
#include<assert.h>
#include <mysql/mysql.h>
#include"persist.h"
#include"my_recv.h"
#include"../client/logic.h"

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
void reg_account_per(int clifd,MYSQL* mysql)
{
	account_t* account=(account_t*)malloc(sizeof(account_t));
	myrecv(clifd,(char*)account,sizeof(account_t));
	char* buf=insert_data(mysql,account->name,account->passwd);
	if((send(clifd,(void*)buf,200,0))<0)
		my_err("send res fail",__LINE__);
	free(buf);
}
/* 连接mysql数据库 */  
void mysql_connect(MYSQL *mysql)
{  
    if(!mysql_real_connect(mysql, HOST, USER, PASSWD, DB_NAME, 0, NULL, 0)) 
    {  
        printf("Failed to connect:%s\n", mysql_error(mysql));
        my_err("mysql_real_connect", __LINE__);
    }  
    printf("Connect database sucessfully!\n\n");
} 
char* insert_data(MYSQL *mysql,char* name,char*passwd)
{  
    int t; 
    char* res=(char*)malloc(200);
    char *head = "insert into ";  
    char query[200];
    char *left = "( ";  
    char *right = " ) ";  
    char *values = "values ";  



    /* 把几个变量字符串连接成一个完整的mysql命令 */  
    sprintf(query, "insert into Accounts (name,passwd) values (%s,%s);",name,passwd);
    printf("%s\n", query);  

    t = mysql_real_query(mysql, query, strlen(query));
    if (t) {  
        sprintf(res,"%s%s","Failed to query:", mysql_error(mysql));  
        return res;
    }
    strcmp(res,"Success add!");
    return res;
}  
void close_connection(MYSQL *mysql)
{
    mysql_close(mysql);
}

