#include <stdio.h>

int *p ,*q;

int swap(int *a,int *b)
{
 int t;
 t = *a;
 *a = *b;
 *b =t;
}

int main()
{
	int q,p;
	p = 22;
	q =33;
swap(&p,&q);
printf("p = %d  q =%d",p,q);
}
