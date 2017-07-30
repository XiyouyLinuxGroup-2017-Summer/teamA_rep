#include<stdio.h>
#include<string.h>

int main()
{
	int n,t=1;
	scanf("%d",&n);
	while(n--)
	{
		int i,j=0,nsum,nbit=0,over=0;
		char A[1010]="",B[1010]="",sum[1011]="";
		scanf("%s %s",A,B);
		int len1=strlen(A);
		int len2=strlen(B);
		char tempA[1010],tempB[1010];
		for(i=len1-1;i>=0;i--)
		{
			tempA[j++]=A[i];
		}
		tempA[j]='\0';
		j=0;
		for(i=len2-1;i>=0;i--)
		{
			tempB[j++]=B[i];
		}
		int maxlen=len1>len2?len1:len2;
		if(len1<len2)
		{
			for(i=len1;i<len2;++i)
				tempA[i]='0';
			tempA[i]='\0';
		}
		else if(len1>len2)
		{
			for(i=len2;i<len1;++i)
				tempB[i]='0';
			tempB[i]='\0';
		}
		for(i=0;i<maxlen;i++)
		{
			nsum=tempA[i]-'0'+tempB[i]-'0'+nbit;
			if(nsum>9)
			{
				if(i==maxlen-1)
					over=1;
				nbit=1;
				sum[i]=nsum-10+'0';
			}
			else
			{
				nbit=0;
				sum[i]=nsum+'0';
			}
		}
		if(over==1)
		{
			sum[maxlen++]=nbit+'0';
		}
		sum[maxlen]='\0';
		
		printf("Case %d:\n",t++);
		printf("%s + %s = ",A,B);
		for(i=maxlen-1;i>=0;i--)
			putchar(sum[i]);
		if(n==0)
			printf("\n");
		else
			printf("\n\n");
		
	
	}
}
