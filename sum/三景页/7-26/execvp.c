#include<stdio.h>
#include <unistd.h>

int main(int argc,char** argv)
{
    execvp("/myls",argv);
}
