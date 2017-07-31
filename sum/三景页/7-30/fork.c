#include<stdio.h>
#include<unistd.h>
#include<wait.h>
#include<sys/types.h>
int i=0;

void ff(int n,int pid)
{
    if(i==5||pid!=0)
        return;
    pid=fork();
    sleep(3);
    ff(n+1,pid);
}
int main()
{
     int pid,statues;
     i++;
     pid=fork();
     ff(1,pid);
    sleep(60);
}

