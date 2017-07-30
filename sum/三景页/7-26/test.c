#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
char a[10][10];     //记录棋盘位置
int book[10];        //记录一列是否已经放过棋子
int n,k;
int total,m;    //total 是放棋子的方案数 ，m是已放入棋盘的棋子数目

void DFS(int cur)
{
    if(k==m)
    {
        total++;
        return ;
    }
    if(cur>=n)    //边界

        return ;
    for(int j=0; j<n; j++)
        if(book[j]==0 && a[cur][j]=='#')  //判断条件
        {
            book[j]=1;           //标记
            m++;                 
            DFS(cur+1);
            book[j]=0;           //改回来方便下一行的判断
            m--;
        }
    DFS(cur+1);                //到下一行
}

int main()
{
    int i,j;
    while(scanf("%d%d",&n,&k)&&n!=-1&&k!=-1) //限制条件
    {
        total=0;
        m=0;
        for(i=0; i<n; i++)
            scanf("%s",&a[i]);
        memset(book,0,sizeof(book));
        DFS(0);
        printf("%d\n",total);
    }
    return 0;
}
