#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
int main()
{
    char* ar[10]={"wc",NULL};
    //int fd=open("aa",O_RDWR|O_CREAT|O_APPEND,0644);
    int fd=open("aa",O_RDONLY);
    dup2(fd,0);
    execvp("wc",ar);
}
