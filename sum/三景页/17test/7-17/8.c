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
		if(n!=0)
			printf("%d\n\n",sum);
		else
			printf("%d\n",sum);
	}
}
