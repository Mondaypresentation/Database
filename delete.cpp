#pragma once 
#include<stdio.h>
#include"page.cpp"
int checkPage_Key(Page *p,char *key,int &offset,int TNo)//页是否有key
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

PageNode* checkBuf_Key(PageTable *PT,BufferQue* Buf,char *key,int &offset,int TNo)//缓冲区是否有key 
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
Page* checkPT_Key(PageTable *PT,char *key,int &offset,int TNo)//页表中是否有key
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
void deleteKeyList(Page *&p,char *key,int offset)//更新页的keylist,删除记录 
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

Block *Rec_OUT_Block(Block *b,int offset,int TNo)//从块中删除记录,表号tableno,块号blockno，记录号offset
{
	//先查看缓冲区内是否有此块
	//更新块信息
	

	if(offset<b->FirstAvailableBlock||offset>=MAX_OF_RECORD)
	{
		printf("错误，块中没有此记录");
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
	int t=0;//t用来标注看块是不是全是空的，t=0时全为空
	
	for(int i=0;i<MAX_OF_RECORD;i++)
	{
		if(b->RecordCondition[i]==1)//若有不是空的记录则跳出
		{
			t=1;
			break;
		}
	}
	//if(t==0) {}如果块全是0可在磁盘中删除块（可不操作，留以后修改）
	//更新缓冲区信息（需要补充）

}

void deleteRecord(PageTable *&PT,BufferQue* &Buf,char *key,int TNo)//从TNo表删除一条记录
{
	//遍历缓冲区的页，是否有key 
	printf("表%s记录%s\n",TableList[TNo],key);
	int offset=-1;
	PageNode *p1 = checkBuf_Key(PT,Buf,key,offset,TNo);
	if(p1){//p1非空，即返回缓冲区中可以key所在的页 
		printf("记录在缓冲区表%s页%d中\n",TableList[TNo],p1->PageID);
		Block *c=p1->block;
		printf("从表%s块%d中删除记录\n",TableList[TNo],c->BlockNo);
		c=Rec_OUT_Block(c,offset,TNo);
		//————————————————- 
		//ID=p1->BlockID;找到对应block b
		//——————————————--  
		
		//printf("1\n"); 
		
		//从块b中删除记录
		deleteKeyList(p1->page,key,offset);//更新页的key索引
		//printf("2\n");
		//BT = Remove(BT,atoi(key));
		printf("删除记录成功\n");
	}
	else{//p1为空,记录不在缓冲区 
		Page *p2 = checkPT_Key(PT,key,offset,TNo);
		if(p2){//p2非空，返回页表中可以插入记录的页 
			//printf("1\n");
			Block *b=FIFO(PT,Buf,p2); 
			//printf("1\n");
			b=Rec_OUT_Block(b,offset,TNo);
			//从块b中删除记录
			
			deleteKeyList(p2,key,offset);//更新页的key索引
			//BT = Remove(BT,atoi(key));
			printf("删除记录成功\n"); 
		}
		else
			printf("数据库中找不到记录%s\n",key);
	}
	//printf("————————————————————————————————————\n");
 } 
