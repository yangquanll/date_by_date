#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct _w{
	int l,wr;
	char *w;
	long lg;
	char ww[1024];

}ww;

typedef struct _Cw{

	int a;
	void *na;
	float b;
	ww sw;
	ww *ppsw;
}Cw;

typedef struct _aw{
	int u;


}AW;

static ww ssw = {

	.l = 11,
	.wr = 33,
	.ww = {0},
	.lg = 999999999999,
}, *pw = &ssw;
// 初始化完，并且定义一个指针初始化为 本struct

static Cw ccw = {

	.a = 999,
	.na = &ssw,
	.b = 123323423,
	.ppsw = &ssw,


}, *pcw = &ccw;


void tw1(ww *w)
{
	printf("wr = %d\n",w->wr);
}

void tst(Cw *cw)
{
	printf( " cwa = %d, cw->na->lg = %ld\n",cw->a,cw->ppsw->lg);

}



int main()
{
	
tst(pcw);
Cw *tmp = (Cw*)malloc(sizeof(Cw));

tw1(tmp);
free(tmp);
tmp = NULL;

}
