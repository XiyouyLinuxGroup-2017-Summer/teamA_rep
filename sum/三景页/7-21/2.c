#include<stdio.h>
#include<stdlib.h>
int main()
{
	int i;
	char **filename=(char**)malloc(sizeof(char*)*300);
	for(i=0;i<5;i++)
	{
		(filename[i])=(char*)malloc(sizeof(char)*100);
	}
}
