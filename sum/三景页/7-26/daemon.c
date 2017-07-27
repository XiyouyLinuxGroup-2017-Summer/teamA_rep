#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<signal.h>
#include<sys/param.h>
#include<sys/stat.h>
#include<time.h>
#include<syslog.h>
#include<stdlib.h>

int init_daemon(void)
{
    int pid;
    int i;

    signal(SIGTTOU,SIG_IGN);
    signal(SIGTTIN,SIG_IGN);
    signal(SIGTSTP,SIG_IGN);
    signal(SIGHUP,SIG_IGN);

    pid=fork();
    if(pid>0)
    {
        exit(0); //结束父进程
    }
    else if(pid<0)
    {
        return -1;
    }
    
    //建立新进程组，让子进程成为会话组长,使该进程脱离所有终端
    setsid();

    //再建立一个子进程，退出父进程,保证不是进程组长，同时使其无法打开新的终端
    pid=fork();
    if(pid>0)
        exit(0);
    else if(pid<0)
        return -1;

    for(i=0;i<NOFILE;close(i++));

    chdir("/");

    umask(0);

    signal(SIGCHLD,SIG_IGN);

    return 0;
}

int main()
{
    time_t now;
    init_daemon();
    syslog(LOG_USER|LOG_INFO,"测试守护进程!\n");
    while(1)
    {
        sleep(6);
        time(&now);
        syslog(LOG_USER|LOG_INFO,"系统时间：\t%s\t\t\n",ctime(&now));
    }
}
