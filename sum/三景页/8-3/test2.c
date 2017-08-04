#include<stdio.h>
#include<string.h>

void cc(char* p)
{
	char t[100005];
	int len=strlen(p),i;
	for(i=0;i<len;i++)
	{
		if(p[i]!='0')
			break;
	}
	strcpy(t,p+i);
	len=strlen(t);
	if(strchr(t,'.')!=NULL)
	{
		for(i=len-1;i>=0;i--)
		{
			if(t[i]!='0')
			{
				t[i+1]='\0';
				break;
			}
		}
	}
	if(t[i]=='.')
		t[i]='\0';
	strcpy(p,t);
}
int main()
{
    char str1[100005],str2[100005];
    while(scanf("%s %s",str1,str2)!=EOF)
    {
    	cc(str1);
    	cc(str2);
    	if(strcmp(str1,str2)==0)
    		puts("YES");
    	else
    		puts("NO");
    }
}
