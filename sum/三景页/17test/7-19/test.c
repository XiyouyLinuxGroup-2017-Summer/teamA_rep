#include<stdio.h>
#include<string.h>

int main()
{
	char str[]="马上期末考试asd";
	int size=sizeof(str);
	int len =strlen(str);
	printf("size=%d ,len=%d \n",size,len);
	printf("%s\n",str);
	for(int i=0;i<len;i++)
		printf("%d  ",str[i]);

}
