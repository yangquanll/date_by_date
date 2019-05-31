#include <iostream>
#include <opfiles.h>

FILES* FilesOperation :: get_file_op(char *name) //函数定义 要放到 FILES*  而不要靠近函数名
{
	assert(name != NULL);
	FILES *file = NULL;
	char data[MAX_DATA_SIZE];

    if(access(name, F_OK))//判断文件的权限
    {
        printf("####L(%d) file:%s not exist!", __LINE__, name);
        //exit(0);
    }
    
    file->name = name;
	FILE *fp = fopen(name, "rb");
    if(NULL == fp )
    {
        printf("file:%s can not open!\n", name);
        //exit(0);
    }
   
	file->data_size = fread(file->data,sizeof(char),MAX_DATA_SIZE,fp);
    printf("file->data_size = %ld\n",file->data_size);

    fseek(fp, 0, SEEK_END); //把文件内部指针移动到文件尾部
    file->data_size  = ftell(fp); //返回指针偏离文件头的位置(即文件中字符个数)
    fseek(fp, 0, SEEK_SET); //把文件内部指针移回到文件头部
    
    fclose(fp);
    return file;
}




