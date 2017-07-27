#include<stdio.h>
#include<stdlib.h>

char desk[10][10];
int a[10];
int n,k,m,t;

void DFS(int cur)
{
    if(m==k)
    {
        t++;
        return;
    }
    if(cur>=k)
        return;
    for(int i=0;i<n;i++)
    {
        if(desk[cur][i]=='#'&&a[i]==0)
        {
            a[i]=1;
            m++;
            DFS(cur+1);
            m--;
            a[i]=0;
        }
        DFS(cur++);
    }
}

int main()
{
    while(scanf("%d %d",&n,&k)&&n!=-1&&k!=-1)
    {
        m=0;t=0;
        int i;
        for(i=0;i<n;i++)
            scanf("%s",desk[i]);
        memset(a,0,sizeof(a));
        DFS(0);
        printf("%d\n",t);
    }

    
}
