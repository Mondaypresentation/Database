#include <stdio.h> 
#include "dirent.h"  

#define  FilePath "/NATION"

int main()  
{     
	int filesize = 0;  
    DIR *dir = NULL;  
    struct dirent *entry;  

    if((dir = opendir(FilePath))==NULL)  
    {  
        printf("opendir failed!");  
        return -1;  
    }
    else  
    {  
        while(entry=readdir(dir))  
        {  
            printf("filename = %s",entry->d_name);  //输出文件或者目录的名称
            printf("filetype = %d\n",entry->d_type);  //输出文件类型   
        }  
entry->
        closedir(dir);    
    }  
    return 0;    
}  

