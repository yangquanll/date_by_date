#include <iostream>
#include <string>
 #include <unistd.h>
#include <sys/wait.h>

using namespace std;
#define CMP_DATA (static_cast <unsigned char> 0)

typedef struct {

	int ret;
	char *rsp;
}mtn;

int main(int argc, char *argv[])
{
	void *p;
	mtn *m;
	m -> ret = 888;
//	m -> rsp = NULL;
//	p = reinterpret_cast <void *> (m);
//	cout<<p-> ret<<endl;

	free(p);
	char buf[1024];
	FILE *fp;
	strings cmd = argv[1];
	fp = popen(cmp.to_str,"r");
	while (fgets(buf, LOG_BUF_MAX_LENGTH, fp)) {
		 resp += linebuf;
	}

	int *r = pclose(fp);
	if (*r)
		*r = WEXITSTATUS(*r);




}

