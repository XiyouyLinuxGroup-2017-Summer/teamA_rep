#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<limits.h>
#include<dirent.h>
#include<grp.h>
#include<pwd.h>
#include<errno.h>

#define PARAM_NONE 0
#define PARAM_A    1
#define PARAM_L    2
#define MAXROWLEN  80

int g_leave_len = MAXROWLEN;
int q_maxlen;

void my_err(const char* err_string,int line)
{
	fprintf(stderr,"line:%d",__LINE__);
	perror(err_string);
	exit(1);
}

void display_single(char* name)
{
	int i,len;
	
	if(g_leave_len<g_maxlen)
	{
		printf("\n");
		g_leave_len=MAXROWLEN;
	}
	len=strlen(name);
	len=g_maxlen-len;
	printf("-%s",name);
	for(i=0;i<len;i++)
	{
		printf("\n");
	}
	printf(" ");
	g_leave_len-=(g_maxlen+2);
}
void display_dir(int flag_param,char* path)
{
	DIR* dir;
	struct dirent* ptr;
	int count=0;
	char filenames[256][PATH_MAX+1],temp[PATH_MAX+1];

	/*获取该目录下文件总数和最长的文件名 */
	dir=opendir(path);
	if(dir==NULL)
		my_err("opendir",__LINE__);
	while((ptr=readdir(dir))!=NULL)
	{
		if(q_maxlen<strlen(ptr->d_name))
			q_maxlen=strlen(ptr->d_name);
		count++;
	}
	closedir(dir);

	if(count>256)
		my_err("too many files under this dir",__LINE__);

	int i,j,len=strlen(path);
	//获取该目录下所有的文件名
	
	dir=opendir(path);
	for(i=0;i<count;i++)
	{
		ptr=readdir(dir);
		if(ptr==NULL)
		{
			my_err("readdir",__LINE__);
		}
		strncpy(filenames[i],path,len);
		filenames[i][len]='\0';
		strcat(filenames[i],ptr->d_name);
		filenames[i][len+strlen(ptr->d_name)]='\0';
	}
	for(i=0;i<count-1;i++)
	{
		for(j=0;j<count-1-i;j++)
		{
			if(strcmp(filenames[i],filenames[i+1])>0)
			{
				strcpy(temp,filenames[i]);
				strcpy(filenames[i],filenames[i+1]);
				strcpy(filenames[i+1],temp);
			}
		}
	}
	for(i=0;i<count;i++)
	{
		//display(flag_param,filenames[i]);
	}
	closedir(dir);
	if((flag_param&PARAM_L)==0)
		printf("\n");
}
int main(int argc,char** argv)
{
	int i,j,k,num;
	k=num=0;
	int flag_param=PARAM_NONE;
	char param[32]="";
	char path[PATH_MAX+1]="";
	for(i=1;i<argc;i++)
	{
		if(argv[i][0]=='-')
		{
			for(j=1;j<strlen(argv[i]);j++)
			{
				param[k]=argv[i][j];
				k++;
			}
			num++;
		}
	}
	param[k]='\0';
	/* 判断参数 */
	for(i=0;i<k;i++)
	{
		if(param[i]=='a')
			flag_param|=PARAM_A;
		else if(param[i]=='l')
			flag_param|=PARAM_L;
		else if(param[i]=='R')
			/********/;
		else
		{
			printf("my_ls:invalid option -%c\n",param[i]);
			exit(0);
		}
	}

	/* 判断未输入文件名或目录名的情况*/
	if(num+1==argc)
	{
		strcpy(path,"./");
		display_dir(flag_param,path);
		return 0;
	}



}
