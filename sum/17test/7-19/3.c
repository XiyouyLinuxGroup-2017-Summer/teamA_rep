#include<stdio.h>
#include<string.h>

int main()
{
	int n,i;
	scanf("%d",&n);
	getchar();
	while(n--)
	{
		char str[1000]="";
		gets(str);
		int len=strlen(str);
		int t=0;
		for(i=0;i<len;i++)
		{
			if(str[i]<0)
				t++;
		}
		printf("%d\n",t/2);
	}
}
