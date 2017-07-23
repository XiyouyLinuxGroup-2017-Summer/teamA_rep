#include<stdio.h>

int main()
{
	int i,n;
	while(scanf("%d",&n),n!=0)
	{
		int arr[100]={0,1,2,3,4};
		i=5;
		if(n<=4)
			printf("%d\n",arr[n]);
		else
		{
			for(;i<=n;i++)
				arr[i]=arr[i-1]+arr[i-3];
			printf("%d\n",arr[n]);
		}
	}
}
