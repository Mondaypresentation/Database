#pragma once 
#include<stdio.h>
#include"page.cpp"
PageTable *PTList[MAX_OF_TABLE];//全局页表
BufferQue *Buf;//全局缓冲区
PageNode* selectBuf_Block(BufferQue* Buf,int Bno,int TNo)//缓冲区是否有块Bno 
{
	PageNode *pn = Buf->front->next;
	while(pn)
	{
		if(pn->block->BlockNo == Bno && pn->block->TableNo == TNo)
			return pn;
		pn=pn->next;
	}
	return NULL;
}
Page* selectPT_Block(PageTable *PT,int Bno)//页表中是否有页Bno，返回页 
{
	Page *p=PT->next;
	while(p){
		if(!p->IFBuffer)
			if(p->BlockID == Bno)
				return p;
		p=p->next;
	}
	
	return NULL;
 }
