#include<stdio.h>

int main()
{
	int n,x,sum,nn;
	scanf("%d",&n);
	while(n--)
	{
		scanf("%d",&nn);
		sum=0;
		while(nn--)
		{
			scanf("%d",&x);
			sum+=x;
		}
		printf("%d\n",sum);
	}
	
}
