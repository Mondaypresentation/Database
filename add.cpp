#include<stdio.h>
#include"page.cpp"
#include "load.cpp"
#include"BPlusTree.cpp"
int checkPage_Free(Page *p,int TNo)//页是否有空闲空间 
{
	if(p->TableNo != TNo)
		return 0;
	for(int t = 0;t < MAX_OF_RECORD;t++)
		if(strlen(p->KeyList[t])==0)
			{return 1;} 
	return 0;
}

PageNode* checkBuf_Free(PageTable *PT,BufferQue* Buf,int TNo)//缓冲区是否有空闲空间 ，返回缓冲区节点 
{
	PageNode *pn = Buf->front->next;
	while(pn)
	{
		Page *p = pn->page;
		if(checkPage_Free(p,TNo)==1)
			return pn;
		pn=pn->next;
	}
	return NULL;
 }
Page* checkPT_Free(PageTable *PT,int TNo)//页表中是否有空闲空间，返回页 
{

	Page *p=PT->next;
	while(p){
		if(!p->IFBuffer)
			if(checkPage_Free(p,TNo)==1)
				return p;
		p=p->next;
	}
	
	return NULL;
 } 
int addKeyList(Page *&p,char *key)//空闲空间分配 
{

	for(int t = MAX_OF_RECORD-1;t>=0;t--)
		{
		if(strlen(p->KeyList[t]) == 0) 
			
			{
				//printf("4\n");
				//p->KeyList[t][8]=key;
				strcpy(p->KeyList[t],key);
				//printf("%d\n",t);
				p->F_Update=1;
				return t;
	    	}
	    }
	
}
Block* Rec_IN_Block(Block *b,char **value,int TNo) //块中添加记录
{
	    char table[255];
		sprintf(table, "%.4d",TNo);
		
		Record *record=load(table,value);
		//strcpy(record->Head->tablenum,table);
		char *a=input (record);
		//将value识别成record 
		//printf("%s\n",a);
		//printf("11111\n");
		if (b->FirstAvailableBlock==-1) 
		{
			printf("error,block %d is full",b->BlockNo);
			return NULL;
		}
		//cur_block=ReadBlock(tableno,FirstBlockAvailble);//从文件中读出表中第一个可用的块作为当前操作的块,cur_block代表当前操作的块(两个参数未定义，应在索引中定义)
		//将块载入缓冲区？
		//下面更新块的信息 
		strcpy(b->Re[b->FirstAvailableBlock],a);//把记录写入首个空的记录区
		
		b->RecordCondition[b->FirstAvailableBlock]=1;//把此区域标记为已使用
		b->LastUsedRecord=b->FirstAvailableBlock; //最后一个被使用的块是此块
		
        b->FirstAvailableBlock=b->FirstAvailableBlock-1;
        free(a); 
        return b;
}
void addRecord(BPlusTree &BT,PageTable *&PT,BufferQue* &Buf,char *key,char** value,int TNo) //向TNo表中增加一条记录 
{
	
	//遍历缓冲区的页，是否有空闲空间
 
	//printf("!!!!!!!!!!!!\n"); 
	int offset = 0;
	PageNode *p1 = checkBuf_Free(PT,Buf,TNo);	
	if(p1){//p1非空，即返回缓冲区中可以插入记录的页 
		//printf("缓冲区中有空闲空间\n");
		//printf("向块%d插入记录\n",p1->block->BlockNo);
		//printf("%d\n",p1->page->PageID);
		Block *c=(Block *)malloc(sizeof(Block));
		c=p1->block;
		c=Rec_IN_Block(c,value,TNo);
		//————————————————- 
			//ID=p1->BlockID;
			//向b中插入char **value 
		//——————————————--  

		
		offset = addKeyList(p1->page,key);//更新页的key索引
		BT = Insert(BT,atoi(key),TNo,c->BlockNo,offset);//插入B+树 
	//	for(int i=0;i<20;i++)
	//	{printf("%s\n",p1->page->KeyList[i]);} 
	}
	else{//p1为空,缓冲区已满 
		//printf("1\n");
		//printf("2\n");
		Page *p2 = checkPT_Free(PT,TNo);//////////////
		//printf("2\n");
		if(p2){//p2非空，返回页表中可以插入记录的页 
			//printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"); 
			Block *b=FIFO(PT,Buf,p2); 
			//printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"); 
			//————————————————- 
			//ID=p2->BlockID;
			//向b中插入char **value 
			//——————————————--
            b=Rec_IN_Block(b,value,TNo);
			//printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"); 
			offset = addKeyList(p2,key);//更新页的key索引
			//printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"); 
			BT = Insert(BT, atoi(key),TNo,b->BlockNo,offset);////插入B+树
			//printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");  
		}
		else
			printf("数据库页表空间已满\n");
	}
	//Block *c=(Block *)malloc(sizeof(Block));
	//c=Buf->front->next->block;
	
}

