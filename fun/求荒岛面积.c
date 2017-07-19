/*************************************************************************
	> File Name: 求荒岛面积.c
	> Author: 
	> Mail: 
	> Created Time: 2017年07月19日 星期三 15时29分22秒
 ************************************************************************/

#include<stdio.h>
int book[21][21];
int sum;
int n,m;
int a[21][21];
void dfs(int x,int y)
{
    int arr[4][2]={{0,1},//向下
                   {0,-1},// 向上
                   {-1,0},//向左
                   {1,0}//向右
                  };
    int tx,ty;
    int i;
    for(i=0;i<4;i++)
    {
        tx=x+arr[i][0];
        ty=y+arr[i][1];
        if(tx<1||tx>n||ty<1||ty>m)//防止越界
            continue;
        if(a[tx][ty]>0&&book[tx][ty]==0)
        {
            book[tx][ty]=1;
            sum++;
            dfs(tx,ty);//注意这个dfs在if里面，因为只有这个点可行，才会在这个点
            //四个方向继续去找
        }
    }
    return ;
}
int main()
{
    int startx,starty;
    int i,j;
    printf("输入几行几列");
    scanf("%d %d",&n,&m);
    for(i=1;i<=n;i++)
        for(j=1;j<=m;j++)
            scanf("%d",&a[i][j]);
    printf("请输入起点");
    scanf("%d %d",&startx,&starty);
    book[startx][starty]=1;
    sum=1;
    dfs(startx,starty);
    printf("%d\n",sum);
}
