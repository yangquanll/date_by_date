#include <stdio.h>
const char *msg[] ={"a","b","c","d","e","f"};

void const get_print(const char **p)
{
	static int i =0;
	printf("i = %d \n",i);
	*p = msg[i%7];
	i++;
}

int main()
{
const char *p_1 = NULL;
const char *p_2 = NULL;

get_print(&p_1);
get_print(&p_2);

printf("p_1= %s p_2= %s \n",p_1,p_2);

}
