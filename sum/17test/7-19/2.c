#include<stdio.h>
#include<math.h>

int p(int* a,int n,int x)
{
	int f=1,i;
	for(i=0;i<n;i++)
	{
		if(x%a[i]!=0)
		{
			f=0;
			return f;
		}
	}
	return f;
}
int main()
{
	int a[100],n,i,t,x;
	while(scanf("%d",&n)!=EOF)
	{
		
		for(i=0;i<n;i++)
		{
			scanf("%d",&a[i]);
		}
		t=a[0];
		for(i=0;i<n;i++)
		{
			if(a[i]>t)
				t=a[i];
		}
		while(1)
		{
			if(x=p(a,n,t))
			{
				break;
			}
			t++;
		}
		printf("%d\n",t);
	}
}
