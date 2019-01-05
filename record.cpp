#pragma once 
#include<stdio.h>
#include<malloc.h>
#include <string.h>
#include"database.h"
typedef struct Attribute{
	int IFVar;//true:变长 false:定长 
	int type;//属性字符类型
	int AttributeLength;//变量长度 
	char name[20];
	char value[255]; 
}attribute;
typedef struct RecordHead{//记录首部 
	//指向模式的指针
	char tablenum[5];//表名 
	int attrubuteNum;//属性个数 
	int RecordLength;//记录长度
	//时间戳 
}RecordHead; 
typedef struct Record{//记录 
	RecordHead *Head;
	struct Attribute *attribute[Max_Attribute_Num];
}Record;

struct Attribute* InitAttribute()
{
	struct Attribute *A = (struct Attribute*)malloc(sizeof(struct Attribute));
	(struct Attribute*)memset(A,0x00,sizeof(struct Attribute));
	A->IFVar = 0;
	A->type = 0;
	A->AttributeLength = 0;
	return A;
}
RecordHead* InitRH(){
	RecordHead *Head = (RecordHead*)malloc(sizeof(RecordHead));
	(RecordHead*)memset(Head,0x00,sizeof(RecordHead));
	Head->attrubuteNum = 0;
	Head->RecordLength = 0;
	return Head;
}
Record* InitRecord()
{
	Record *R = (Record*)malloc(sizeof(Record));
	(Record*)memset(R,0x00,sizeof(Record));
	R->Head = InitRH();
	for(int i=0;i<Max_Attribute_Num;i++){
		R->attribute[i] = InitAttribute();
	}
	return R;
}
Record* SetRecord(int num)
{
	Record *R = InitRecord();
	int i=0;
	for(i=0;i<num;i++)
	{
    strcpy(R->attribute[i]->name,"Init");
	R->attribute[i]->type = 0;
	R->attribute[i]->IFVar = 0;
	R->attribute[i]->AttributeLength =0;		
	}
	return R;
} 
Record* InputRecord(Record *R,int NO,int type)
{
	R->attribute[NO]->type = type;
	return R;
 } 
