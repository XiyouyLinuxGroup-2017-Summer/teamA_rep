#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main()
{
	int k;
	char* msg;
	pid_t pid;
	printf("process creation %d,%d\n",getpid(),getppid());
	pid=fork();

		switch(pid)
		{
			case 0:
			//printf("c p r ,C is %d,P is %d,M is %d\n",pid,getppid(),getpid());
			msg="Child running";
			k=5;
			break;
			case -1:
			perror("failed\n");
			default:
			k=3;
			msg="Parent running";
			//printf("P is running,C is %d,myself is %d\n",pid,getpid());
		}
		//printf("%d\n",n);
		while(k>0)
		{
			puts(msg);
			sleep(1);
			k--;
		}
}
