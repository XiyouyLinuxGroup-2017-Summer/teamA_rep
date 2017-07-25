#include<stdio.h>

int gcd(int x,int y)
{
	return y==0?x:gcd(y,x%y);
}
int main()
{
	int a,b,n,c;
	scanf("%d",&n);
	while(n--)
	{
		scanf("%d %d",&a,&b);
		c=2*b;
		while(1)
		{
			if(gcd(a,c)==b)
				break;
			c+=b;		
		}
		printf("%d\n",c);
	
	}
}
