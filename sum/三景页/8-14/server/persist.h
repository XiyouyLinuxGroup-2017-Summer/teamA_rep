#pragma once

#define HOST    "localhost"
#define USER    "root"
#define PASSWD  "Asd892058773asd."
#define DB_NAME "chatroom"

void myrecv(int clifd,char* buf,int len);
void reg_account_per(int clifd,MYSQL* mysql);
void lading_ser(int clifd,MYSQL* mysql);

void mysql_connect(MYSQL *mysql);                   // 连接mysql数据库
char* insert_data(MYSQL *mysql,char* name,char* passwd);                     // 插入数据
int match_account(MYSQL *mysql,char* name,char* passwd);	 //账号匹配
void close_connection(MYSQL *mysql);                // 关闭mysql数据库
int insert_Online(MYSQL* mysql,char* name,int clifd);                  //插入在线列表
void print_table(MYSQL *mysql);				//显示表中内容
void mysql_initOnline(MYSQL* mysql); 		//初始化在线列表
void delete_data(MYSQL *mysql,char* table,char* name);   //删除表中数据
