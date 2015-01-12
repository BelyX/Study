#include<stdio.h>
int main()
{
FILE* fp;
fp = fopen("1.txt","w+");
int i = 0;
for(i = 0; i<1024*1024*1024; i++)
{
fputc('1',fp);
}
}
