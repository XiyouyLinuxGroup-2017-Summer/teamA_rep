#include<stdio.h>  
 
const int maxn = 17;  
const int MAXN = 100017;  
  
int main()  
{  
    int n;  
    int cas = 0;  
    int a[maxn], b[maxn], c[maxn];  
    while(scanf("%d",&n)&&n)  
    {  
        for(int i = 1; i <= n; i++)  
            scanf("%d%d%d", &a[i], &b[i], &c[i]);  
        int ans;  
        int count = 0;  
          
        for(ans = 1; ans < MAXN; ans++)  
        {  
            count = 0;  
            for(int i = 1; i <= n; i++)  
            {  
                if(c[i] <= a[i]) 
                    count++; 
            }  
            if(count == n)     
                break;  
            for(int i = 1; i <= n; i++)  
            {  
                if(c[i] == a[i]+b[i] || (c[i] == a[i] && count > n-count))  
                {  
                    c[i] = 0;
                }  
                c[i]++;  
            }  
        }  
        if(ans == MAXN)  
            ans = -1;  
  
        printf("Case %d: %d\n", ++cas, ans);  
    }  
    return 0;  
}

