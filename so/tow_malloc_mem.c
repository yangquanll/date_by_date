#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct 
{
	int num;
	char *msg;
	
}yq_s;

void alloc(yq_s **pp)
{
	yq_s *p = (yq_s*)malloc(sizeof(yq_s));
	if(p == NULL) {
		printf("out of memory\n");
		exit(1);
	}
	p->num = 3;
	p->msg = (yq_s*)malloc(sizeof(p->msg));
	printf("p->msg size  = %d",p->msg);
	strcpy(p->msg, "Hello World!");
	*pp = p;
}

void free_unit(yq_s *p)
{
	printf("free \n");
	free(p->msg);
	free(p);
}

int main()
{
	yq_s *y;
	alloc(&y);
	printf("y->msg = %s\n",y->msg);
	free_unit(y);
	y = NULL;
	return 0;

}


