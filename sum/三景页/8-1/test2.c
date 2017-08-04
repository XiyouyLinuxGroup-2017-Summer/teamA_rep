#include<stdio.h>
#include<string.h>

int problem[16][16],vis[16];
int n,max;

void dfs(int line,int pre,int s1)
{
    int flag=0,i;
    for(i=0;i<n;i++)
    {
        if(i==line||vis[i]==1)
            continue;
        if(problem[line][i]>=pre)
        {
            vis[i]=1;
            dfs(i,problem[line][i],s1+1);
            vis[i]=0;
            flag=1;
        }
    }
    if(!flag)
        max=s1>max?s1:max;
}

int main()
{
    int i,j;
    while(scanf("%d",&n)!=EOF)
    {
        memset(vis,0,sizeof(vis));
        for(i=0;i<n;i++)
            for(j=0;j<n;j++)
                scanf("%d",&problem[i][j]);
        vis[0]=1;
        max=-1;
        dfs(0,0,1);
        printf("%d\n",max);
    }
}
