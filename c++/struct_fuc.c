#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

typedef int STRU_FU;

STRU_FU fun1(int a, const char *b){

	printf("fun1  a = %d , b = %s \n",a,b);

	return a;
}

STRU_FU fun2(int a, void *b,float c){

	printf("fun2  a = %d , b = %f \n",a,c);

	return a;
}

STRU_FU fun3(int a, char b,long c){

	printf("fun3  a = %d , b = %ld \n",a,c);
	return a;
}

struct fun_ops{

	STRU_FU (*fun1)(int a, char *b);
	STRU_FU (*fun2)(int a, void *b,float c);
	STRU_FU (*fun3)(int a, char b,long c);
};

struct fun_ops fuops = {

	.fun1 = fun1,
	.fun2 = fun2,
	.fun3 = fun3,
};

int main(int argc,  char* argv[])
{
	int g;
	struct fun_ops *ops = NULL; 
	if(strcmp(argv[1], "1") == 0)
	{
		ops = &fuops;//赋值结构体
		g =1;
		printf("fun1 \n");
		ops->fun1(111,"fun1");
	}else if( !strcmp(argv[1], "2")){

		ops = &fuops;
		g= 2;
		printf("fun2 \n");
		ops->fun2(2222,"fun2",3333);

	}else{

		ops = &fuops;
		g = 3;
		printf("fun3 \n");
		ops->fun3(1212,"y",9999999);

	}
	}
