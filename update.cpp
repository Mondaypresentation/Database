#include <stdio.h>
#include "add.cpp" 
#include "delete.cpp"
void UpdateRecord(PageTable *&PT,BufferQue* &Buf,char *key1,char *key2,char** value,char *table)//删除键值为key1的记录，增添键值为key2的记录 
{
	deleteRecord(PT,Buf,key1);
	addRecord(PT,Buf,key2,value,table); 
}
