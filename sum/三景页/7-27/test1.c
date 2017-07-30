#include<stdio.h>
//#include<math.h>

double input()
{
    double n,x,y;
    double sum=0;
    scanf("%lf %lf",&n,&x);
    while(n--)
    {
        scanf("%lf",&y);
        sum=sum*x+y;
    }
    return sum;
}
int main()
{
    double a,b;
    a=input();
    b=input();

    if(a>b)
        puts(">");
    else if(a==b)
        puts("=");
    else
        puts("<");
}
