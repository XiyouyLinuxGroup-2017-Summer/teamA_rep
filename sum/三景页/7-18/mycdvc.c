#include<stdio.h>
#include<unistd.h>
#include<limits.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>

void my_err(const char* err_string,int line)
{
	fprintf(stderr,"line:%d ",line);
	perror(err_string);
	exit(1);
}

int main(int argc, char** argv)
{
	char buf[PATH_MAX+1];
	int fd;
	if(argc<2)
	{
		printf("my_cd <taget path>\n");
		exit(1);
	}
	if(chdir(argv[1])<0)
	{
		my_err("chdir",__LINE__);
	}
	if(fd=open("exampletest.c",O_RDWR|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR)<0)
	{
		my_err("open",__LINE__);
	}
	if(getcwd(buf,512)<0)
	{
		my_err("getcwd",__LINE__);
	}
	printf("%s \n",buf);

	return 0;
}
