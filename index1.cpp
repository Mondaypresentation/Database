#pragma once 
#include<stdio.h>
#include<malloc.h>
#include <string.h>
#include"charbplustree.cpp"
#include"BPlusTree.cpp"
void createIntTree(BPlusTree &BT,PageTable *&PT,BufferQue* &Buf,int TNo,int ANo) 
{
	clearBuf(Buf);//清空缓冲区
	Page *P = PT->next;//页表中首页 
	
	Record *R= SetRecord(TNo);
	while(P){
		if(strlen(P->KeyList[MAX_OF_RECORD-1]) != 0)
		{
			Block *b=FIFO(PT,Buf,P);
		//printf("!!!!!!!!!2\n");
			for(int i = MAX_OF_RECORD-1;i>=0;i--){
				if(strlen(P->KeyList[i])==0)
					break;
			//printf("!!!!!!!!!3\n");
				char s[500];
			//	sprintf(s,"%.4d%s",TNo,b->Re[i]);
			//	printf("%s\n",b->Re[i]);
				R=output(b->Re[i]);
			//printf("!!!!!!!!!4\n");
			//	printf("%d\n",atoi(R->attribute[ANo]->value));
				BT = Insert(BT,atoi(R->attribute[ANo]->value),TNo,P->BlockID,i);
			
			//R->attribute[1]->value
			//TNo,P->BlockID,i
			}
		}
		
		P = P->next;
			
	}
		
} 

//为char字段建立索引
void createCharTree(CharBPlusTree &BT,PageTable *&PT,BufferQue* &Buf,int TNo,int ANo) 
{
	clearBuf(Buf);//清空缓冲区
	Page *P = PT->next;//页表中首页 
	
	Record *R= SetRecord(TNo);
	while(P){
		if(strlen(P->KeyList[MAX_OF_RECORD-1]) != 0)
		{
			Block *b=FIFO(PT,Buf,P);
		//printf("!!!!!!!!!2\n");
			for(int i = MAX_OF_RECORD-1;i>=0;i--){
				if(strlen(P->KeyList[i])==0)
					break;
			//printf("!!!!!!!!!3\n");
				char s[500];
			//	sprintf(s,"%.4d%s",TNo,b->Re[i]);
			//	printf("%s\n",b->Re[i]);
				R=output(b->Re[i]);
			//printf("!!!!!!!!!4\n");
			//	printf("%d\n",atoi(R->attribute[ANo]->value));
				BT = InsertChar(BT,R->attribute[ANo]->value,TNo,P->BlockID,i);
			
			//R->attribute[1]->value
			//TNo,P->BlockID,i
			}
		}
		
		P = P->next;
			
	}
		
} 

