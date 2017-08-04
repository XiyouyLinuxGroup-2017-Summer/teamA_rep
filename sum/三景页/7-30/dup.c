#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
int main()
{
    int pid,sfd;
    int statues;
    char* argv1[10]={"ls",NULL};
    char* argv2[10]={"wc",NULL};
    int fd = open("file",O_RDWR|O_CREAT|O_TRUNC,0644);
    sfd=dup(1);
    dup2(fd,1);
    if((pid=fork())==0)
    {
         execvp(argv1[0],argv1);
    }
    if(waitpid(pid,&statues,0)==-1)
    {
        printf("wait for child process error\n");
    }
    fd=open("file",O_RDONLY);
    dup2(fd,0);
    dup2(sfd,1);
    //dup2(1,fd); //错误做法
    execvp(argv2[0],argv2);
}
