#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<errno.h>

int main(int argc,char** argv)
{
	int mode;
	char* path;
	if(argc<3)
	{
		printf("%s <mode num> <target file>\n",argv[0]);
		exit(0);
	}
	mode=strtol(argv[1],NULL,8);
	if(mode>0777||mode<0)
	{
		printf("mode num error!\n");
		exit(0);
	}
	path=argv[2];
	if(chmod(path,mode)==-1)
	{
		perror("chmod error!");
		exit(1);
	}
	return 0;
}
