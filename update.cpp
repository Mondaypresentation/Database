#include <stdio.h>
#include "add.cpp" 
#include "delete.cpp"
void UpdateRecord(PageTable *&PT,BufferQue* &Buf,char *key1,char *key2,char** value,char *table)//ɾ����ֵΪkey1�ļ�¼�������ֵΪkey2�ļ�¼ 
{
	deleteRecord(PT,Buf,key1);
	addRecord(PT,Buf,key2,value,table); 
}
