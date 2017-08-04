#include <iostream>  
#include <string.h>  
using namespace std;  
int problem[16][16],vis[16];  
int n,maxx;  
  
void dfs(int line,int pre,int sl)  
{  
    int i,flag=0;  
    for(i=0;i<n;++i)  
    {  
        // 当前i是否访问过，或者i是否就是当前站点  
        if(i==line || vis[i]==1)    continue;  
  
        if(problem[line][i]>=pre)  
        {  
            vis[i]=1;  
            dfs(i,problem[line][i],sl+1);  
            vis[i]=0;  
            flag=1;  
        }  
    }  
    if(!flag)  maxx=sl>maxx?sl:maxx;  
}  
  
int main()  
{  
    int i,j;  
    while(cin>>n)  
    {  
        memset(vis,0,sizeof(vis));  
        for(i=0;i<n;++i)  
            for(j=0;j<n;++j)  
                cin>>problem[i][j];  
  
        // 初始化maxx，开始dfs  
        vis[0]=1;  
        maxx=-1;  
        dfs(0,0,1);  
        cout<<maxx<<endl;  
    }  
    return 0;  
}  
