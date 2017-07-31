#include<stdio.h>
#include<string.h>
#define MAX 10
int a[MAX][MAX]={0},way[100][2]={0},p,q;
int dx[8]={-2,-2,-1,-1,1,1,2,2},   
    dy[8]={-1, 1,-2,2,-2,2,-1,1};
int tol;
int dfs(int rear,int x,int y)
{
    if(rear==tol)
        return 1;
    else
        for(int i=0;i<8;i++)
        {
            int nx=x+dx[i],ny=y+dy[i];
            if(nx>=0&&nx<q&&ny>=0&&ny<p&&a[nx][ny]==0)
            {
                a[nx][ny]=1;
                if(dfs(rear+1,nx,ny))
                {
                    way[rear][0]=nx;
                    way[rear][1]=ny;
                    return 1;
                }
                a[nx][ny]=0;
            }
        }
    return 0;
}
int main()
{
    int n;
    scanf("%d",&n);
    for(int i=1;i<=n;i++)
    {
        memset(a,0,sizeof(a));
        a[0][0]=1;
        scanf("%d %d",&p,&q);
        tol=p*q;
        if(i!=1)
            printf("\n");
        printf("Scenario #%d:\n",i);
        if(dfs(1,0,0))
        {
            for(int j=0;j<tol;j++)
            {
             printf("%c%d",way[j][0]+'A',way[j][1]+1);
            }
            printf("\n");
        }
        else
            printf("impossible\n");
    }
}
