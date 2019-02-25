#include <stdio.h>
int add_ret() ;
int multiply();

int add(int a , int b , int (*add_value)()) //
{
	return (*add_value)(a,b);
}

int multiplk_p(int c,int d,int (*multip_date)())
{
	return (*multip_date)(c,d);
}

int main(void)
{
	int sum = add(3,4,add_ret);
    int multip = multiplk_p(6,5,multiply);
	printf("sum:%d\n",sum);
	printf("mutil:%d\n",multip);
	return 0 ;
} 

int add_ret(int a , int b)
{
	return a+b ;
}

int multiply (int c, int d)
{
printf("c = %d ,d =%d \n ",c,d);
return c - d;
}
