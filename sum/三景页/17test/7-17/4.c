#include<stdio.h>

int main()
{
	int n,x,sum;
	while(scanf("%d",&n),n!=0)
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
