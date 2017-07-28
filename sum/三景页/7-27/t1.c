#include<stdio.h>
#include<unistd.h>

int main()
{
    char* ar[10]={"cat","myshell.c",">","jieguo",NULL};
    execvp("cat",ar);
}
