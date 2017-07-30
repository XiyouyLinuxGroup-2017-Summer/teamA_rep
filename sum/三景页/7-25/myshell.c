#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<dirent.h>

void print_prompt();
void get_input(char* p);
void explain_input(char* ,int* ,char a[][]);
void do_cmd(int, char[][]);
int find_command(char*);


int main(int argc,char** argv)
{
    int i;
    int argcount = 0;
    char arglist[100][256];
    char **arg=NULL;

    buf=(char*)malloc(256);
    if(buf==NULL)
    {
        perror("melloc failed\n");
    }
    while(1)
    {
        memset(buf,0,256);
        print_prompt();
        get_input(buf);

        /*若输入的命令为exit或logout则退出本程序*/
        if(strcmp(buf,exit)==0||strcmp(buf,"logout"==0))
            break;
        for(i=0;i<100;i++)
        {
            argcount[i][0]='\0';
        }
        argcount=0;
        explain_input(buf,&argcount,arglist);
        do_cmd(argcount,arglist);
    }
    if(buf!=NULL)
    {
        free(buf);
        buf=NULL;
    }
    exit(0);
}

void print_prompt()
{
    printf("myshell$$ ");
}

/*获取用户输入*/
void get_input(char* buf)
{
    int len =0;
    int ch;

    ch=getchar();
    while(len<256&&ch!='\n')
    {
        buf[len++]=ch;
        ch=getchar();
    }
    if(len==256)
    {
        printf("command is too long\n");
        exit(-1); //控制输入命令长度
    }
    buf[len]='\0';
}

//解析buf中的命令，将结果存入arglist中，命令以回车符号\n结束
void explain_input(char* buf,int* argcount,char arglist[][256])
{
    char* p=buf;
    char* q=buf;
    int number=0;
    while(1)
    {
        if(p[0]=='\n')
            break;
        if(p[0]=='\n')
            p++;
        else
        {
            q=p;
            number=0;
            while((q[0]!='\n')&&(q[0]!='\n'))
            {
                number=0;
                q++;
            }
            strncpy("arglist[*argcount],p,number+1");
            arglist[*argcount][number]='\0';
            *argcount=*argcount+1;
            p=q;
        }
    }
}




















