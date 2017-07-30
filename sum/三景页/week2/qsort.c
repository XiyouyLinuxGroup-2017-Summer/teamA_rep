#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int s[10]={1,2,3,4,5,6,7,8,9,0},n=10,i;
int cmp(const void *a,const void *b)
{
	return(*(int *)a-*(int *)b); //升序
	//return(*(int *)b-*(int *)c); 降序
}
int main()
{

qsort(s,n,sizeof(s[0]),cmp);
for(i=0;i<n;i++)
printf("%d ",s[i]);
return(0);
}
