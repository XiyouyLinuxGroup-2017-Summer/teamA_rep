#include<stdio.h>
int a[6]={100,50,10,5,2,1};
int count;
void cc(int n)
{
    int i;
    for(i=0;i<6;i++)
    {
        if((n/a[i])>=1)
        {
        	int f=n/a[i];
            count+=f;
            n=n-f*a[i];
        }
    }
}
int main()
{
    int n,x;
    
    while(scanf("%d",&n)!=EOF,n!=0)
    {
    	count=0;
        while(n--)
        {
            scanf("%d",&x);
            cc(x);
        }
        printf("%d\n",count);
    }
}
