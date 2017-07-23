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
#include<stdlib.h>

#define PARAM_NONE 0        //由于后面要用到位运算，所以介绍一下，当没有参数的时候flag=0；然后依次是-a,-l,-R,-r，定义为1,2,4,8
#define PARAM_A    1        //刚好就是二进制中的1，10，100，1000。这样方便为运算中的|和&，比如同时有a和r参数，那么flag就为1001.
#define PARAM_L    2		//当要判断是否含有这两个参数中的一个时(比如r参数)就可以用flag&PARAM_r,如果为0的话就没有r这个参数。
#define PARAM_R    4		//其他的也类似
#define PARAM_r    8
#define MAXROWLEN  80

char PATH[PATH_MAX+1];
int flag;

int g_leave_len = MAXROWLEN;
int g_maxlen;

void my_err(const char* err_string,int line);
void display_dir(char* path);

void my_err(const char* err_string,int line)
{
	fprintf(stderr,"line:%d",__LINE__);
	perror(err_string);
	exit(1);
}

void cprint(char* name,mode_t st_mode)
{

	if(S_ISLNK(st_mode))   //链接文件
	    printf("\033[1;36m%-*s\033[0m",g_maxlen,name);
	else if(S_ISDIR(st_mode)&&(st_mode&000777)==0777)   //满权限的目录
		printf("\033[1;34;42m%-*s  \033[0m",g_maxlen,name);
	else if(S_ISDIR(st_mode))  //目录
                printf("\033[1;34m%-*s  \033[0m",g_maxlen,name);
	else if(st_mode&S_IXUSR||st_mode&S_IXGRP||st_mode&S_IXOTH) //可执行文件
		printf("\033[1;32m%-*s  \033[0m",g_maxlen,name);
	else   //其他文件
		printf("%*s  ",g_maxlen,name);
}
void  display_attribute(char* name)  //-l参数时按照相应格式打印
{
	struct stat buf;
	char buff_time[32];
	struct passwd* psd;  //从该结构体接收文件所有者用户名
	struct group* grp;   //获取组名
	if(lstat(name,&buf)==-1)
	{
		my_err("stat",__LINE__);
	}
	if(S_ISLNK(buf.st_mode))
		printf("l");
	else if(S_ISREG(buf.st_mode))
		printf("-");
	else if(S_ISDIR(buf.st_mode))
		printf("d");
	else if(S_ISCHR(buf.st_mode))
		printf("c");
	else if(S_ISBLK(buf.st_mode))
		printf("b");
	else if(S_ISFIFO(buf.st_mode))
		printf("f");
	else if(S_ISSOCK(buf.st_mode))
		printf("s");
	//获取打印文件所有者权限
	if(buf.st_mode&S_IRUSR)
		printf("r");
	else
		printf("-");
	if(buf.st_mode&S_IWUSR)
		printf("w");
	else
		printf("-");
	if(buf.st_mode&S_IXUSR)
		printf("x");
	else
		printf("-");

	//所有组权限
	if(buf.st_mode&S_IRGRP)
		printf("r");
	else
		printf("-");
	if(buf.st_mode&S_IWGRP)
		printf("w");
	else
		printf("-");
	if(buf.st_mode&S_IXGRP)
		printf("x");
	else
		printf("-");
	
	//其他人权限
	if(buf.st_mode&S_IROTH)
		printf("r");
	else
		printf("-");
	if(buf.st_mode&S_IWOTH)
		printf("w");
	else
		printf("-");
	if(buf.st_mode&S_IXOTH)
		printf("x");
	else
		printf("-");

	printf("  ");
	//根据uid和gid获取文件所有者的用户名和组名
	psd=getpwuid(buf.st_uid);
	grp=getgrgid(buf.st_gid);
	printf("%4d ",buf.st_nlink);  //链接数
	printf("%-8s ",psd->pw_name);
	printf("%-8s ",grp->gr_name);

	printf("%6d",buf.st_size);
	strcpy(buff_time,ctime(&buf.st_mtime));
	buff_time[strlen(buff_time)-1]='\0'; //buff_time自带换行，因此要去掉后面的换行符
	printf("  %s  ",buff_time);
	cprint(name,buf.st_mode);
	printf("\n");
}
void  displayR_attribute(char* name)  //当l和R都有时，先调用display_attribute打印，然后该函数负责递归
{
	struct stat buf;
	
	if(lstat(name,&buf)==-1)
	{
		my_err("stat",__LINE__);
	}
	if(S_ISDIR(buf.st_mode))
	{
			display_dir(name);
			free(name);
			char* p=PATH;
			while(*++p);
			while(*--p!='/');
			*p='\0';
			chdir("..");
			return;
	
	}
}
void display_single(char* name)   //打印文件
{
	int len ;
	struct stat buf;
	if(lstat(name,&buf)==-1)
	{
		return;
	}

	if(g_leave_len<g_maxlen)
	{
		printf("\n");
		g_leave_len=MAXROWLEN;
	}
	
	cprint(name,buf.st_mode);  //根据文件的不同类型显示不同颜色
	g_leave_len=g_leave_len-(g_maxlen+2);
	
}

void displayR_single(char* name)  //当有-R参数时打印文件名并调用display_dir
{
	int len ;
	struct stat buf;
	if(lstat(name,&buf)==-1)
	{
		return;
	}
	if(S_ISDIR(buf.st_mode))    
	{
			printf("\n");
			
	    	g_leave_len=MAXROWLEN;
	    	
			display_dir(name);
			free(name);   //将之前filenames[i]中的空间释放
			char*p=PATH;
			
			while(*++p);
			while(*--p!='/');
			*p='\0';
			chdir("..");  //返回上层目录
	}
	
}

void display(char **name ,int count)  //根据flag去调用不同的函数
{
	switch(flag)
	{
		int i;
		case PARAM_r:
		case PARAM_NONE:
			for(i=0;i<count;i++)
			{
				if(name[i][0]!='.') //排除.., .,以及隐藏文件
					display_single(name[i]);
			}
			break;
		case PARAM_r+PARAM_A:
		case PARAM_A:
			for(i=0;i<count;i++)
			{
				display_single(name[i]);
			}
			break;
		case PARAM_r+PARAM_L:
		case PARAM_L:
		for(i=0;i<count;i++)
		{
			if(name[i][0]!='.')
			{
				display_attribute(name[i]);
				
			}
		}
			break;
		case PARAM_R+PARAM_r:
		case PARAM_R:
			for(i=0;i<count;i++)
			{
				if(name[i][0]!='.')
				{
					display_single(name[i]);
				}
			}
			for(i=0;i<count;i++)
			{
				if(name[i][0]!='.')   //排除 "."和".."两个目录，防止死循环，下同
				{
					displayR_single(name[i]);
				}
			}
			break;
		case PARAM_L+PARAM_r+PARAM_R:
		case PARAM_R+PARAM_L:
			for(i=0;i<count;i++)
			{
				if(name[i][0]!='.')
				{
					display_attribute(name[i]);
				}
			}
			for(i=0;i<count;i++)
			{
				if(name[i][0]!='.')
				{
					displayR_attribute(name[i]);
				}
			}
			break;
		case PARAM_A+PARAM_r+PARAM_R:
		case PARAM_R+PARAM_A:
			for(i=0;i<count;i++)
			{
					display_single(name[i]);
			}
			for(i=0;i<count;i++)
			{
				if(name[i][0]!='.')
				{
					displayR_single(name[i]);
				}
			}
			break;

		case PARAM_A+PARAM_L+PARAM_r:
		case PARAM_A+PARAM_L:
			for(i=0;i<count;i++)
			{
				display_attribute(name[i]);
			}
			break;
		case PARAM_A+PARAM_L+PARAM_R+PARAM_r:
		case PARAM_A+PARAM_L+PARAM_R:
			for(i=0;i<count;i++)
			{
				display_attribute(name[i]);
			}
			for(i=0;i<count;i++)
			{
				if(name[i][0]!='.')
				{
					displayR_attribute(name[i]);
				}
			}
			break;
		default:
			break;
	}
}


void display_dir(char* path)      //该函数用以对目录进行处理
{
	DIR* dir;                     //接受opendir返回的文件描述符
	struct dirent* ptr;           //接受readdir返回的结构体
	int count=0;
	//char filenames[300][PATH_MAX+1],temp[PATH_MAX+1];  这里是被优化掉的代码，由于函数中定义的变量
	//是在栈上分配空间，因此当多次调用的时候会十分消耗栈上的空间，最终导致栈溢出，在linux上的表现就是核心已转储
	//并且有的目录中的文件数远远大于300

	if((flag&PARAM_R)!=0)
	{
		if(path[0]=='.')
		{
			strcat(PATH,path);
		}
		else
		{
			strcat(PATH,"/");
			strcat(PATH,path);
		}
		printf("%s:\n",PATH);
	}
	//获取文件数和最长文件名长度 
	dir = opendir(path);
	if(dir==NULL)
		my_err("opendir",__LINE__);
	g_maxlen=0;
	while((ptr=readdir(dir))!=NULL)
	{
		if(g_maxlen<strlen(ptr->d_name))
			g_maxlen=strlen(ptr->d_name);
		count++;
	}
	closedir(dir);
	char **filenames=(char**)malloc(sizeof(char*)*count),temp[PATH_MAX+1];  //通过目录中文件数来动态的在堆上分配存储空间，首先定义了一个指针数组
	for(int i=0;i<count;i++)												//然后依次让数组中每个指针指向分配好的空间，这里是对上面的优化，有效
	{																		//的防止了栈溢出，同时动态分配内存，更加节省空间
		filenames[i]=(char*)malloc(sizeof(char)*PATH_MAX+1);
	}
	
	int i,j;
	//获取该目录下所有的文件名
	dir=opendir(path);
	for(i=0;i<count;i++)
	{
		ptr=readdir(dir);
		if(ptr==NULL)
		{
			my_err("readdir",__LINE__);
		}
		strcpy(filenames[i],ptr->d_name);
	}
	closedir(dir);
	//冒泡法对文件名排序
	if(flag&PARAM_r)  //-r参数反向排序
	{
		for(i=0;i<count-1;i++)
		{
			for(j=0;j<count-1-i;j++)
			{
				if(strcmp(filenames[j],filenames[j+1])<0)
				{
					strcpy(temp,filenames[j]);
					strcpy(filenames[j],filenames[j+1]);
					strcpy(filenames[j+1],temp);
				}
			}
		}
	}
	else //正向排序
	{
		for(i=0;i<count-1;i++)
                {
                        for(j=0;j<count-1-i;j++)
                        {
                                if(strcmp(filenames[j],filenames[j+1])>0)
                                {       
                                        strcpy(temp,filenames[j]);
                                        strcpy(filenames[j],filenames[j+1]);
                                        strcpy(filenames[j+1],temp);
                                }
                        }
                }

	}

	if(chdir(path)<0)
	{
		my_err("chdir",__LINE__);
	}
	
	display(filenames,count);
	if((flag&PARAM_L==0&&!(flag|PARAM_R)))
		printf("\n");
}

int main(int argc,char** argv)
{
	int i,j,k=0,num=0;
	char param[32]="";     //用来保存命令行参数
	char *path[1];		   //保存路径，其实我也不想定义为一个指针数组，但是为了和后面的char **类型的函数参数对应只能定义成这样
	path[0]=(char*)malloc(sizeof(char)*(PATH_MAX+1));   //由于是一个指针类型，所以我们要给他分配空间  PATH_MAX是系统定义好的宏，它的值为4096,严谨一点，加1用来存'\0'
	flag=PARAM_NONE;	   //初始化flag=0（由于flag是全剧变量，所以可以不用初始化）
	struct stat buf;       //该结构体以及lstat,stat，在 sys/types.h，sys/stat.h，unistd.h 中，具体 man 2 stat
	
	//命令行参数解析,将-后面的参数保存至param中
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
			flag|=PARAM_A;
		else if(param[i]=='l')
			flag|=PARAM_L;
		else if(param[i]=='R')
			flag|=PARAM_R;
		else if(param[i]=='r')
			flag|=PARAM_r;
		else
		{
			printf("my_ls:invalid option -%c\n",param[i]);
			exit(0);
		}
	}
	
	//如果没有输入目标文件或目录，就显示当前目录
	if(num+1==argc)
	{
		strcpy(path[0],".");
		display_dir(path[0]);
		return 0;
	}

	i=1;
	do
	{
		if(argv[i][0]=='-')
		{
			i++;
			continue;
		}
		else
		{
			strcpy(path[0],argv[i]);
			//判断目录或文件是否存在
			if(stat(argv[i],&buf)==-1)
			{
				my_err("stat",__LINE__);    //编译器内置的宏，插入当前源码行号
			}
			if(S_ISDIR(buf.st_mode))  //判断是否为目录
			{
				display_dir(path[0]);
				i++;
			}
			else
			{
				display(path,1);
				i++;
			}
		}
	}while(i<argc);
	printf("\n");

	return 0;

}
