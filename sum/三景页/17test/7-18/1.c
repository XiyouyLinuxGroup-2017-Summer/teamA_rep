#include<stdio.h>
#include<string.h>
int main()
{
	char str[110]="";
	int f,i,len,t;
	while(scanf("%s",str)!=EOF)
	{
		t=f=0;
		len=strlen(str);
		for(i=0;i<len;i++)
		{
			if(str[i]=='@')
				t++;
		}
		if(str[0]=='.'||str[0]=='@'||str[len-1]=='.'||str[len-1]=='@')
		{
			printf("NO\n");
			continue;
		}
		if(t>1||t==0)
		{
			printf("NO\n");
			continue;
		}
		char* p1=strchr(str,'@');
		if(*(p1+1)=='.'||*(p1-1)=='.')
		{
			printf("NO\n");
			continue;
		}
		char* p2=strchr(p1,'.');
		if(p2==NULL)
		{
			printf("NO\n");
			continue;
		}
		printf("YES\n");

	}
}
