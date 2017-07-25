#include<stdio.h>
int gcd(int a,int b)
{
	return b==0?a:gcd(b,a%b);
}

int lcm(int a,int b)
{
	return a/gcd(a,b)*b;
}

int main()
{
	int x1,y1,x2,y2,x,y;
	int n;
	scanf("%d",&n);
	while(n--)
	{
		scanf("%d/%d",&x1,&y1);
		scanf("%d/%d",&x2,&y2);
		int g1=gcd(x1,y1);
		int g2=gcd(x2,y2);
		x1/=g1;y1/=g1;
		x2/=g2;y2/=g2;
		x=lcm(x1,x2);
		y=gcd(y1,y2);
		if(x%y==0)
			printf("%d\n",x/y);
		else
		{
			x1=gcd(x,y);
			x/=x1;
			y/=x1;
			printf("%d/%d\n",x,y);
		}
	}

}
