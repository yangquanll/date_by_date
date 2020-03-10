#include <stdio.h>
//#include <errno.h>
//#include <string.h>
#include <stdlib.h>
#include <unistd.h> //close()
#define FILE_NAME  "./K706G_4275_0000000000.bin"

int main()
{
    printf("11111111111111111 \n");
	FILE *file = fopen(FILE_NAME, "rb");

	if(file == NULL)
	{
		printf("open failed\n");
	}
	printf("2\n");

	char keyboxStr[10024];
	int r =0;

	 if ((r = fseek(file, 0L, SEEK_END))) {
        printf("seek(%s) failed with %d\n", FILE_NAME, r);
    }
	printf("3\n");

	long len = ftell(file);
    if (len < 0) {
        printf("ftell(%s) failed: %s(%d)\n", FILE_NAME, strerror(-r), -r);
    }

    printf("File %s length %ld \n", FILE_NAME, len);
    rewind(file);

    printf("File length %ld beyond range\n", len);
/*
    if ((keyboxStr = malloc(len)) == NULL) {
        printf("malloc(%ldBytes) failed", len);
    }
*/
    size_t read_len = fread(keyboxStr, sizeof(char), len, file);
if(read_len == 0)
{
	printf("fread(%s, len=%ld) failed. Read %u bytes", FILE_NAME, len, read_len);
}
  
#if 1
	int i = 0;
	for (i = 0; i < 128; i += 8) {
    printf("0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x \n",keyboxStr[i + 0], keyboxStr[i + 1], keyboxStr[i + 2], keyboxStr[i + 3],keyboxStr[i + 4], keyboxStr[i + 5],  keyboxStr[i + 6], keyboxStr[i + 7]);    
    }

//	printf("0x%02x \n",keyboxStr[8 + 1]);
#endif
 fclose(file);

}
