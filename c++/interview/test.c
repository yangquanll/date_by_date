#include<stdio.h>
#define BIT (0x1 << 3)
#include <stdlib.h>
int main()
{
int a = 10;
int b,c,d;
b = a++;
//printf("a++ = %d \n",a++);
c = ++a;
d = 10*a++;
printf("a = %d , b =%d ,c = %d ,c = %d \n",a,b,c, d);

int i,y;
i =i|BIT;
y = i;
y =y&~BIT;
char *st;
itoa(i,st,2);
printf(" a =%d  y = %d \n ",i,y);

}

