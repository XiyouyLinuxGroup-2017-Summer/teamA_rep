#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int qvar=5;

int main()
{	
	pid_t pid;
	int var=1,i,k=10;
	char str1[20];
	char* str2=(char*)malloc(20);
	gets(str1);
	gets(str2);
	//char* p=str1;
	printf("fork is different with vfork\n");
	//pid=fork();
	pid=vfork();
	switch(pid)
	{
		case 0:
			i=3;
			while(i--)
			{
				printf("Child process is running\n");
				qvar++;
				var++;
				sleep(1);
			}
			puts(str1);
		//	puts(p);
			puts(str2);
			printf("Child's qvar = %d,var = %d\n",qvar,var);
			
			break;
		case -1:
			perror("failed\n");
			exit(0);
			break;
		default:
			i=5;
			while(i-->0)
			{
				printf("Parent is running\n");
				qvar++;
				var++;
				sleep(1);
			}
			puts(str1);
		//	puts(p);
			puts(str2);
			printf("Parent's qvar = %d,var = %d,k=%d\n",qvar,var,k);
			exit(0);
	}



}

