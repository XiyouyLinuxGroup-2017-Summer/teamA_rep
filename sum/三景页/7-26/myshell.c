#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<dirent.h>

#define normal          0   //一般命令
#define out_redirect    1   //输出重定向
#define in_redirect     2   //输入重定向
#define have_pipe       3   //输入有管道命令

void print_prompt();
void get_input(char* p);
void explain_input(char*buf ,int*argcount ,char arglist[][257]);
void do_cmd(int argcount, char arglist[][257]);
int find_command(char* command);


int main(int argc,char** argv)
{
    int i;
    int argcount = 0;
    char arglist[100][257];
    char **arg=NULL;

    char* buf=(char*)malloc(257);
    if(buf==NULL)
    {
        perror("melloc failed\n");
        exit(-1);
    }
    while(1)
    {
        memset(buf,0,256);
        print_prompt();
        get_input(buf);

        /*若输入的命令为exit或logout则退出本程序*/
        if(strcmp(buf,"exit")==0||strcmp(buf,"logout")==0)
            break;
        for(i=0;i<100;i++)
        {
            arglist[i][0]='\0';
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
void explain_input(char* buf,int* argcount,char arglist[][257])
{
    char* p=buf;
    char* q=buf;
    int number=0;
    while(1)
    {
    	if(p[0]=='\0')
    		break;
        if(p[0]==' ')
            p++;
        else
        {
            q=p;
            number=0;
            while(q[0]!=' '&&q[0]!='\0')
            {
                number++;
                q++;
            }
            strncpy(arglist[*argcount],p,number+1);
            arglist[*argcount][number]='\0';
            *argcount=*argcount+1;
            p=q;
        }
    }

}
void do_cmd(int argcount,char arglist[][257])
{
    int flag =0;
    int how= 0;   		// <,>,|总计数
    int background=0;  //后台运行符计数
    int status;
    int i;
    int fd;
    char* arg[argcount+1];
    char* argnext[argcount+1];
    char* file;
    pid_t pid;

    //取出命令
    for(i=0;i<argcount;i++)
    {
        arg[i]=arglist[i];
    }
    arg[argcount]=NULL;

    //查看命令行是否有后台运行符
    for(i=0;i<argcount;i++)
    {
        if(strcmp(arg[i],"&")==0)
        {
            if(i==argcount-1)
            {
                background=1;
                arg[argcount-1]=NULL;
                break;
            }
            else
            {
                printf("wrong command\n");
                return;
            }
         }
    }
    for(i=0;arg[i]!=NULL;i++)
    {
        if(strcmp(arg[i],">")==0)
        {
            flag++;
            how=out_redirect;
            if(arg[i+1]==NULL)
                flag++;
        }
        if(strcmp(arg[i],"<")==0)
        {
            flag++;
            how=in_redirect;
            if(i==0)
                flag++;
        }
        if(strcmp(arg[i],"|")==0)
        {
        	flag++;
        	how=have_pipe;
        	if(arg[i+1]==NULL)
                flag++;
            if(i==0)
                flag++;
        }
    }
    
    //flag大于1说明命令中含有多个>,<,|等符号
    if(flag>1)
    {
        printf("wrong command\n");
        return;
    }

    if(how==out_redirect) //命令只含有一个输入重定向符号">"
    {
        for(i=0;arg[i]!=NULL;i++)
        {
           if(strcmp(arg[i],">")==0)
           {
                file=arg[i+1];
                arg[i]=NULL;
           } 
        }
    }

    if(how==in_redirect)
    {
        for(i=0;arg[i]!=NULL;i++)
        {
            if(strcmp(arg[i],"<")==0)
            {
                file=arg[i+1];
                arg[i]=NULL;
            }
        }
    }

    if(how==have_pipe)
    {
        for(i=0;arg[i]!=NULL;i++)
        {
            if(strcmp(arg[i],"|")==0)
            {
                arg[i]=NULL;
                int j;
                for(j=i+1;arg[j]!=NULL;j++)
                {
                    argnext[j-i-1]=arg[j];
                }
                argnext[j-i-1]=arg[j];
                break;
            }
        }
    }

    if((pid=fork())<0)
    {
        printf("fork error\n");
        return;
    }

    switch(how)
    {
        case 0:
            if(pid==0)
            {
                if(!(find_command(arg[0])))
                {
                    printf("%s : command not found\n",arg[0]);
                    exit(0);
                }
            execvp(arg[0],arg);
            exit(0);
            }
            break;
    }
    

    
}

int find_command(char* command)
{
	
    DIR* dp;
    struct dirent* dirp;
    char* path[]={"./","/bin","/usr/bin",NULL};

    if(strncmp(command,"./",2)==0)
    {
        command=command+2;
    }

    int i=0;
    while(path[i]!=NULL)
    {
        if((dp=opendir(path[i]))==NULL)
        {
            printf("can not open /bin \n");
        }
        while((dirp=readdir(dp))!=NULL)
        {
            if(strcmp(dirp->d_name,command)==0)
            {
                closedir(dp);
                command-=2;
                return 1;
            }
        }
        closedir(dp);
        i++;
    }
    command-=2;
    return 0;

}





















