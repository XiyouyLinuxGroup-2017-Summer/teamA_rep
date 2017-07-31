#include<stdio.h>
#include<readline/history.h>
#include<readline/readline.h>

int main()
{
    while(1)
    {
        char* pro;
        char*print="sanjingye$:";
        pro=readline(print);
        add_history(pro);
        
    }
}
