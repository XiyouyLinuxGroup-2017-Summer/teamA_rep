#include<stdio.h>

int main()
{
	int n,x,sum;
	while(scanf("%d",&n)!=EOF)
	{
		sum=0;
		while(n--)
		{
			scanf("%d",&x);
			sum+=x;
		}
		printf("%d\n",sum);
	}
}
