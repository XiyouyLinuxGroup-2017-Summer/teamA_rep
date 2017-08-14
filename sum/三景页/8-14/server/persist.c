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
/*登陆*/
void lading_ser(int clifd,MYSQL* mysql)
{
	account_t* account=(account_t*)malloc(sizeof(account_t));
	myrecv(clifd,(char*)account,sizeof(account_t));
	if(match_account(mysql,account->name,account->passwd)&&insert_Online(mysql,account->name,clifd))
	{
		if((send(clifd,"success",200,0))<0)
			my_err("send res fail",__LINE__);
		print_table(mysql);
		delete_data(mysql,"Online",account->name);
	}
	else
	{
		if((send(clifd,"fail",200,0))<0)
			my_err("send res fail",__LINE__);
	}
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
    char query[200]; 

    /* 把几个变量字符串连接成一个完整的mysql命令 */  
    sprintf(query, "insert into Accounts (name,passwd) values (\"%s\",\"%s\");",name,passwd);
    printf("%s\n", query);  

    t = mysql_real_query(mysql, query, strlen(query));
    if (t) {  
        sprintf(res,"%s%s","Failed to query:", mysql_error(mysql));  
        return res;
    }
    strcpy(res,"Success add!");
    return res;
}  
int match_account(MYSQL *mysql,char* name,char* passwd)
{
	int t ;
	MYSQL_RES *res;
	char query[200];
	sprintf(query,"select * from Accounts where name=\"%s\" and passwd=\"%s\"",name,passwd);
	printf("%s\n", query); 
	t = mysql_real_query(mysql, query, strlen(query));
	if (t) 
	{
		printf("Failed to query:%s\n", mysql_error(mysql));  
        return 0;
    }
    res = mysql_store_result(mysql);
    mysql_free_result(res);
    return 1;
}
int insert_Online(MYSQL* mysql,char* name,int clifd)
{
	int t; 
    char* res=(char*)malloc(200); 
    char query[200]; 

    /* 把几个变量字符串连接成一个完整的mysql命令 */  
    sprintf(query, "insert into Online (name,fd) values (\"%s\",%d);",name,clifd);
    printf("%s\n", query);  

    t = mysql_real_query(mysql, query, strlen(query));
    if (t) {  
        sprintf(res,"%s%s","Failed to query:", mysql_error(mysql));  
        puts(res);
        return 0;
    }
    strcpy(res,"Success add!");
    puts(res);
    return 1;
}
//显示表中内容
void print_table(MYSQL *mysql)  
{  
    int t;
    char query[50];
    MYSQL_RES *res;         // 返回查询结果
    MYSQL_ROW row;          // 返回行数据

    memset(query, '\0', sizeof(query));

    sprintf(query, "select * from Online");

    t = mysql_real_query(mysql, query, strlen(query));  

    if (t) {
        printf("Failed to query: %s\n", mysql_error(mysql));  
        return;
    }  
    printf("\nQuery successfully!\n\n");  

    res = mysql_store_result(mysql);
    while (row = mysql_fetch_row(res)) {  
        for(t = 0; t < mysql_num_fields(res); t++) {  
            printf("%s\t", row[t]);  
        }  
        printf("\n");
    }  
    mysql_free_result(res);
}  
void mysql_initOnline(MYSQL* mysql)
{
	int t;
    char query[50];
    MYSQL_RES *res;         // 返回查询结果
    MYSQL_ROW row;          // 返回行数据

    memset(query, '\0', sizeof(query));

    sprintf(query, "truncate table Online");

    t = mysql_real_query(mysql, query, strlen(query));  

    if (t) {
        printf("Failed to Init: %s\n", mysql_error(mysql));  
        return;
    }  
    printf("\nInit successfully!\n\n");  

    res = mysql_store_result(mysql);
    mysql_free_result(res);
}
//删除数据
void delete_data(MYSQL *mysql,char* table,char* name)
{  
    int t;
    char query[200];     
    MYSQL_RES *res;         // 返回查询结果
    MYSQL_ROW row;          // 返回行数据

    sprintf(query, "delete from %s where name =\"%s\"", table,name);
    printf("%s\n", query);

    t = mysql_real_query(mysql, query, strlen(query));
    if (t) {  
        printf("Failed to delete: %s\n", mysql_error(mysql));  
        return;
    }
    printf("Delete data sucessfully!\n");  
}  
void close_connection(MYSQL *mysql)
{
    mysql_close(mysql);
}

