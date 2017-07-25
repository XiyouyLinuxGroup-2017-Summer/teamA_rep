#include<stdio.h>
#include<string.h>
#include<stdlib.h>
char str[1000010];
int main()
{
	char str1[]="aab" ; //"Bulbasaur";
	while(scanf("%s",str)!=EOF)
	{
		getchar();
		int len=strlen(str1),i=0,t=0;
		char* p=str;
		while(*p)
		{
			if(strchr(str1[i],*p))
			{
				*p=' ';
				i++;
				if(i==len)
				{
					i%=len;
					t++;
					p=str;
				}
			}
			p++;
	
		}
		memset(str,0,sizeof(str));
		printf("%d\n",t);
	}

}
