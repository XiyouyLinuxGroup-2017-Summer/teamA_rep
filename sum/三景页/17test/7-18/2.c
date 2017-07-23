#include<stdio.h>

int main()
{
	float a,n,max,min,sum,t;
	while(scanf("%f",&n)!=EOF)
	{
		sum=0;
		t=n;
		scanf("%f",&a);
		min=max=a;
		sum+=a;
		while(--n)
		{
			scanf("%f",&a);
			sum+=a;
			if(max<a)
				max=a;
			if(min>a)
				min=a;
		}
		printf("%.2f\n",(sum-max-min)/(t-2));
	}
}
