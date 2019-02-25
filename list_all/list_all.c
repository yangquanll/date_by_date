#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN  sizeof (list_init)

typedef struct student
{
	int num;
	float score;
	struct student *next;
}list_init;

int n;

list_init *creat()
{
	list_init *head;
	list_init *p1=NULL;
	list_init *p2=NULL;
 	p1 = (list_init *)malloc(sizeof(LEN));
	if(p1 == NULL)
	{
		printf("sizeof failed \n");
		return NULL;
	}
	head = NULL;
	p2 = p1;
   printf("node = %d  ",n+1);
    scanf("%d,%f",&(p1->num),&(p1->score));
	while( p1->num != 0)
	{
		n =+ 1;
		if(n == 1)
		{
			head =p1;
			p2->next = NULL;
		}
		else
		{
		 p2 -> next =p1;
		}
		p2 = p1;
		p1 = (list_init *)malloc(sizeof(LEN));
		printf("input %d \n",n+1);
		scanf("%d,%f",&(p1->num),&(p1->score));
	}
	p2 ->next = NULL;
	free(p1);
	p1 = NULL;
	return head;
}


list_init print(list_init *head)
{
  list_init *p;
  p = head;
  if(head != NULL)
  {
	printf("head print = %o p = %o \n",head,p);
	do
	{
		printf("p = %o  p->num = %d, p->score = %f \n",p,p->num,p->score,p->next);
		p= p->next;
	}
	while(p != NULL);

  }
}

int main()
{
	list_init *head,*st;
	head = creat();
	print(head);

return 0;
}
