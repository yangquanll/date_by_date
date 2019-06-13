#include <iostream>
#include <opfiles.h>
#define SAFE_DELETE(p) {if((p) != NULL) delete [] (p); (p) = NULL;}

FILES FilesOperation :: get_file_op(char *name) //函数定义 要放到 FILES*  而不要靠近函数名
{
	assert(name != NULL);
	//FILES *files = (FILES*)malloc(sizeof(FILES*)); //结构体指针 必须初始化； 用非指针的方式 不用初始化
    FILES files;
	char data[MAX_DATA_SIZE];
	printf("file_name = %s\n",name);

    if(access(name, F_OK))//判断文件的权限
    {
        printf("####L(%d) file:%s not exist!", __LINE__, name);
        exit(0);
    }
    
    files.name = name;
	FILE *fp;
	fp = fopen(name, "rb");
    printf("file.data_size = %ld,file.name = %s \n",files.data_size,files.name);
	if(NULL == fp )
    {
        printf("file:%s can not open!\n", name);
        //exit(0);
    }
   
#if 1
    fseek(fp, 0, SEEK_END); //把文件内部指针移动到文件尾部
    files.data_size  = ftell(fp); //返回指针偏离文件头的位置(即文件中字符个数)
	printf("file.data_size ftell()= %ld\n",files.data_size);
    fseek(fp, 0, SEEK_SET); //把文件内部指针移回到文件头部
#endif    
	//bzero(files.data, MAX_DATA_SIZE);
	//files.data_size = fread(files.data,sizeof(char),MAX_DATA_SIZE,fp);
    fclose(fp);
	//SAFE_DELETE(files);
    return files;
	
}
