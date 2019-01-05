#pragma once 
#include<stdio.h>
#include"page.cpp"
PageTable *PTList[MAX_OF_TABLE];//ȫ��ҳ��
BufferQue *Buf;//ȫ�ֻ�����
PageNode* selectBuf_Block(BufferQue* Buf,int Bno,int TNo)//�������Ƿ��п�Bno 
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
Page* selectPT_Block(PageTable *PT,int Bno)//ҳ�����Ƿ���ҳBno������ҳ 
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
