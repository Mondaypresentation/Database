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
            printf("filename = %s",entry->d_name);  //����ļ�����Ŀ¼������
            printf("filetype = %d\n",entry->d_type);  //����ļ�����   
        }  
entry->
        closedir(dir);    
    }  
    return 0;    
}  

