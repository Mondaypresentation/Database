#pragma once 
#include<stdio.h>
#include"page.cpp"
int checkPage_Key(Page *p,char *key,int &offset,int TNo)//ҳ�Ƿ���key
{
	if(p->TableNo != TNo)
		return 0;
	for(int t = 0;t < MAX_OF_RECORD;t++)
		if(strcmp(p->KeyList[t],key) == 0)
		{
			offset=t;
			return 1;
		}
			
	return 0;
}

PageNode* checkBuf_Key(PageTable *PT,BufferQue* Buf,char *key,int &offset,int TNo)//�������Ƿ���key 
{
	PageNode *pn = Buf->front->next;
	while(pn)
	{
		Page *p = pn->page;
		if(checkPage_Key(p,key,offset,TNo)==1)
			return pn;
		pn=pn->next;
	}
	return NULL;
}
Page* checkPT_Key(PageTable *PT,char *key,int &offset,int TNo)//ҳ�����Ƿ���key
{
	Page *p=PT->next;
	while(p){
		if(!p->IFBuffer)
			if(checkPage_Key(p,key,offset,TNo)==1)
				return p;
		p=p->next;
	}
	return NULL;
 }
void deleteKeyList(Page *&p,char *key,int offset)//����ҳ��keylist,ɾ����¼ 
{
	int i=offset;
	for(i;i>0;i--)
		{
		memset(p->KeyList[i],0x00,8*(sizeof(char)));
		strcpy(p->KeyList[i],p->KeyList[i-1]);
		}
	p->F_Update=1;
	
	
	return;

}

Block *Rec_OUT_Block(Block *b,int offset,int TNo)//�ӿ���ɾ����¼,���tableno,���blockno����¼��offset
{
	//�Ȳ鿴���������Ƿ��д˿�
	//���¿���Ϣ
	

	if(offset<b->FirstAvailableBlock||offset>=MAX_OF_RECORD)
	{
		printf("���󣬿���û�д˼�¼");
		return NULL;
	}
	else
	{
		int i=offset;
		for(i;i>b->FirstAvailableBlock;i--)
		{
		memset(b->Re[i],0x00,255*(sizeof(char)));
		strcpy(b->Re[i],b->Re[i-1]);
		}
		b->RecordCondition[b->FirstAvailableBlock]=0;
		b->FirstAvailableBlock++;
	}
	int t=0;//t������ע�����ǲ���ȫ�ǿյģ�t=0ʱȫΪ��
	
	for(int i=0;i<MAX_OF_RECORD;i++)
	{
		if(b->RecordCondition[i]==1)//���в��ǿյļ�¼������
		{
			t=1;
			break;
		}
	}
	//if(t==0) {}�����ȫ��0���ڴ�����ɾ���飨�ɲ����������Ժ��޸ģ�
	//���»�������Ϣ����Ҫ���䣩

}

void deleteRecord(PageTable *&PT,BufferQue* &Buf,char *key,int TNo)//��TNo��ɾ��һ����¼
{
	//������������ҳ���Ƿ���key 
	printf("��%s��¼%s\n",TableList[TNo],key);
	int offset=-1;
	PageNode *p1 = checkBuf_Key(PT,Buf,key,offset,TNo);
	if(p1){//p1�ǿգ������ػ������п���key���ڵ�ҳ 
		printf("��¼�ڻ�������%sҳ%d��\n",TableList[TNo],p1->PageID);
		Block *c=p1->block;
		printf("�ӱ�%s��%d��ɾ����¼\n",TableList[TNo],c->BlockNo);
		c=Rec_OUT_Block(c,offset,TNo);
		//��������������������������������- 
		//ID=p1->BlockID;�ҵ���Ӧblock b
		//����������������������������--  
		
		//printf("1\n"); 
		
		//�ӿ�b��ɾ����¼
		deleteKeyList(p1->page,key,offset);//����ҳ��key����
		//printf("2\n");
		//BT = Remove(BT,atoi(key));
		printf("ɾ����¼�ɹ�\n");
	}
	else{//p1Ϊ��,��¼���ڻ����� 
		Page *p2 = checkPT_Key(PT,key,offset,TNo);
		if(p2){//p2�ǿգ�����ҳ���п��Բ����¼��ҳ 
			//printf("1\n");
			Block *b=FIFO(PT,Buf,p2); 
			//printf("1\n");
			b=Rec_OUT_Block(b,offset,TNo);
			//�ӿ�b��ɾ����¼
			
			deleteKeyList(p2,key,offset);//����ҳ��key����
			//BT = Remove(BT,atoi(key));
			printf("ɾ����¼�ɹ�\n"); 
		}
		else
			printf("���ݿ����Ҳ�����¼%s\n",key);
	}
	//printf("������������������������������������������������������������������������\n");
 } 
