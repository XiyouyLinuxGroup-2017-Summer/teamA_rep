#include<stdio.h>
#include<string.h>

typedef struct str
{
	char world[20];
	int num;
}str;
int main()
{
	int n;
	str qq[1005];
	while(scanf("%d",&n)!=EOF,n!=0)
	{
		int i,j;
		memset(qq,0,sizeof(qq));
		for(i=0;i<n;i++)
			scanf("%s",qq[i].world);
		for(i=0;i<n;i++)
			for(j=0;j<n;j++)
				if(strcmp(qq[i].world,qq[j].world)==0)
					qq[i].num++;
		j=0;
		for(i=1;i<n;i++)
			if(qq[i].num>qq[j].num)
				j=i;
		printf("%s\n",qq[j].world);
	}

}
