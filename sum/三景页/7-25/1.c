#include<stdio.h>
int main()
{
    int a[10]={1,2,3,4,5,6,7,8,9,0};
    int i,j;
    for(i=0;i<10-i-1;i++)
        for(j=0;j<10-1-i;j++)
            printf("%d ",a[j]);
}
