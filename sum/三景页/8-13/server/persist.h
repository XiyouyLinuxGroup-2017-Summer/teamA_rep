#pragma once

#define HOST    "localhost"
#define USER    "root"
#define PASSWD  "Asd892058773asd."
#define DB_NAME "chatroom"

void myrecv(int clifd,char* buf,int len);
void reg_account_per(int clifd,MYSQL* mysql);

void mysql_connect(MYSQL *mysql);                   // 连接mysql数据库
char* insert_data(MYSQL *mysql,char* name,char* passwd);                     // 插入数据
void close_connection(MYSQL *mysql);                // 关闭mysql数据库
