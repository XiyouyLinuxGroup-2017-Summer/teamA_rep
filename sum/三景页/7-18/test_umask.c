#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int main()
{
	umask(0);
	if(creat("example_681.test",0777)<0)
	{
		perror("creat");
		exit(1);
	}
	umask(S_IRWXO);
	if(creat("example_682.test",0777)<0)
	{
		perror("creat");
		exit(1);
	}
	return 0;

}
