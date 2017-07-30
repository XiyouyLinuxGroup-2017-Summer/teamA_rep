#include<stdio.h>

int main()
{
	char str[100];
	char ch;
	ch=getchar();
	int i=0;
	str[i++]=ch;
	while(i<50,ch!='#')
	{
		ch=getchar();
		str[i++]=ch;
	}
	str[i]='\0';
	printf("%s\n",str);
}
