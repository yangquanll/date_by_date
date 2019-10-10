#include<stdio.h>

enum st
{
	x1,
	x2,
	x3 = 10,
	x4,
	x5,
}x;

int main()
{
	printf("st = %d",x);
	int a =6,b=7;

	a^=b;
	b^=a;
	a^=b;

printf("a^b = %d,b^a =%d a = %d,b= %d\n",a^b,b^a,a,b); 
//		printf(" a = %d,b= %d\n",a,b); 
}
