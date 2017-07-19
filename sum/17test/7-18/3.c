#include<stdio.h>

int main()
{
	int n,m,sum,i,f,t,p;
	while(scanf("%d %d",&n,&m)!=EOF)
	{
		i=2;t=0;
		p=n/m;
		while(p--)
		{
			sum=0;
			for(;i<=2*(m+m*t);i+=2)
				sum+=i;
			t++;
			if(p==0&&(n%m)==0)
				printf("%d",sum/m);
			else
				printf("%d ",sum/m);
		}
		if(n%m==0)
		{
			printf("\n");
			continue;
		}
		sum=0;
		f=i;
		for(;i<=f+2*(n%m-1);i+=2)
			sum+=i;
		printf("%d\n",sum/(n%m));
	}
}
