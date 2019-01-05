#pragma once 
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include"database.h"
//#include"BPlusTree.cpp"
/*
typedef structs
{
}Record;//记录结构体 
*/
//#define TableNO 1


typedef struct 
{   
	int BlockNo;//块号
	int TableNo;//所属表号
    int LastUsedRecord;//最后一个被使用的记录
	bool RecordCondition[MAX_OF_RECORD];//标记记录是否被使用 使用为1，未使用为0\
	//Record Re[MAX_OF_RECORD];//记录
	char Re[MAX_OF_RECORD][255];
	int FirstAvailableBlock;//第一个可用的记录空间
}Block;//块的结构体 

typedef struct Page{//页 
	int PageID;//页号 
	int TableNo;
	bool IFBuffer;//是否在主存 
	int BlockID;//块号
	bool CreateBlock;//是否创建块 
	char KeyList[MAX_OF_RECORD][8];//主键列表 
	unsigned char F_Update;//是否更新 
	Page *next;
}Page;

typedef struct{//页表 
	Page *next;//首页 
	int PageNum;//页的数量
	int TableNo;//表号
	//bool PT_Update;//是否更新 
}PageTable;

typedef struct PageNode{//缓冲区中页节点
	int TableNo;//节点对应的表号 
	int PageID;// 节点对应的页号
	Page *page;//节点对应的页 
	Block *block; //节点对应的块
	PageNode *next; 
	
}PageNode;

typedef struct{//缓冲区队列 
	PageNode *front;//指向头结点 
	PageNode *rear;//指向队尾 
	int BufferLength;//缓冲区最大长度 
	int length; //当前长度 
}BufferQue;

void PrintBlock(Block *block) 
{
	printf("%d\n%d\n%d\n%d\n",block->BlockNo,block->FirstAvailableBlock,block->LastUsedRecord,block->TableNo);
	for (int i=0;i<MAX_OF_RECORD;i++)
	{
		printf("%s\n",block->Re[i]);
	}
} 
int SaveBlock(Block block)//存储块，传入参数是块的结构体，可自动存入文件
{
    FILE *f;
    char filepath[255];
    sprintf(filepath,"%s//data",TableList[block.TableNo]);
    //printf("%d\n",block.BlockNo); 
    f = fopen(filepath, "rb+");//以rb+方式打开表二进制文件（需调整文件名）
    if(f==NULL)
    {
       f = fopen(filepath,"w+");
       fclose(f);
       f = fopen(filepath, "rb+");
	}
    if(f != NULL)
	{ 
		fseek(f, (block.BlockNo )* SIZE_OF_BLOCK, 0);//把块写入文件中第BlockNo的位置
        fwrite(&block, sizeof(Block), 1, f);
        fclose(f);
        return 0;
    }
}

Block ReadBlock(int tablenumber,int blocknumber) //从第tableno个表中第blockno块出读出块，返回块的结构体,可先从索引表中找到块的表号和块号
{
	FILE *f;
	char filepath[255];
    sprintf(filepath,"%s//data",TableList[tablenumber]);
    f = fopen(filepath, "rb+");//以rb+方式打开表二进制文件（需调整）
    if(f==NULL)
    {
       f = fopen(filepath,"w+");
       fclose(f);
       f = fopen(filepath, "rb+");
	}
    if(f != NULL)
	{
		Block block;
        fseek(f, blocknumber* SIZE_OF_BLOCK, 0);
        fread(&block, sizeof(Block), 1, f);
        fclose(f);
        return block;
    }
}
Block * NewBlock(int tableNo,int blockNo)//申请新块，主要用于表中块都已满时申请新的块空间，返回块指针
{
    /*if()
    {
       表中块数量已达最大时创建新表或输出错误 
    }*/
	Block *newblock=(Block*) malloc(sizeof(Block));//申请块空间
    newblock->BlockNo = blockNo;//块编号
    newblock->TableNo = tableNo;//块所属表号
    newblock->LastUsedRecord = 0;//块最后一个被使用的记录
    newblock->FirstAvailableBlock =MAX_OF_RECORD-1;
    int i = 0;
    for(i = 0; i < MAX_OF_RECORD; i++)
    {
        newblock->RecordCondition[i]=0;
    }
    //将块头信息写到块空间的头部
    return newblock;
}

int IfBlockIsFull(Block *block)//判断是否块是满的,如果已满则返回1，若未满则返回0
{
	if(block->FirstAvailableBlock==-1) return 1;
	else return 0;
}

Page* InitPage(int i,int TNo)//页初始化 
{
	Page* P = (Page*)malloc(sizeof(Page));
	(Page*)memset(P,0x00,sizeof(Page));
	P->PageID = i;
	P->TableNo = TNo;
	P->IFBuffer = false;
	P->BlockID = i; 
	P->F_Update = 0;
	P->CreateBlock =false;
	P->next = NULL;
	for(int t = 0;t < MAX_OF_RECORD;t++)
		strcpy(P->KeyList[t],"") ;
	return P;
}

PageTable* InitPT(int TNo){//页表初始化 
	PageTable* PT = (PageTable*)malloc(sizeof(PageTable));
	(PageTable*)memset(PT,0x00,sizeof(PageTable));
	PT->TableNo = TNo;
	//————————————页的连接顺序 
	for(int i=PAGENUM-1;i>=0;i--){
		Page *p = InitPage(i,TNo);
		p->next = PT->next;
		PT->next = p;
		//printf("%d",i);
	}
		
	return PT;
}
PageNode *InitPN(){//缓冲区页节点初始化 
	PageNode* PN = (PageNode*)malloc(sizeof(PageNode));
	(PageNode*)memset(PN,0x00,sizeof(PageNode));
	PN->PageID = -1;
	PN->page = NULL;
	PN->block = NULL;////////////////////////////////
	PN->next = NULL;
	return PN; 
}

BufferQue* InitBuffer(){//缓冲区初始化 
	BufferQue* BQ = (BufferQue*)malloc(sizeof(BufferQue));
	(BufferQue*)memset(BQ,0x00,sizeof(BufferQue));

	PageNode *PN = InitPN();
	BQ->front = PN;
	BQ->rear = BQ->front;
	BQ->BufferLength = BufLength;
	BQ->length = 0;
	return BQ; 
}
void printBuf(BufferQue *Buf){//打印缓冲区信息 
	printf("当前缓冲区队列【");
	PageNode *pn = Buf->front->next;
	for(int i = 0;i<Buf->length;i++)
	{
		printf("%d ",pn->PageID);
		pn = pn->next;
	}
	printf("】\n") ;
	
}
Page *SearchPT(PageTable *PT,int ID){//在页表中找到指定页 
	Page *p = PT->next;
	while(p){
		if(p->PageID == ID){
			//printf("在页表中找到页%d\n",ID);
			return p;
		}
		p = p->next;
	}
}
void UpdatePage(PageTable *&PT,Page *&p,int option){//1:写入内存 0：写出内存
	
	if(option == 1){//
		p->IFBuffer = true;
	} 
	else if(option == 0){
		p->IFBuffer = false;
		p->F_Update = 0;
	}
}
Block* FIFO(PageTable *&PT,BufferQue* &Buf,Page *&p_in){//缓冲区队列添加页,返回页对应的block
	//p_in->CreateBlock = true;
	/*if(Buf->BufferLength == 1){
		Buf->front->next = p_in;
	}*/
	if(Buf->length == Buf->BufferLength){//缓冲区队列已满 
		//printf("缓冲区队列已满\n");
		//printf("%d\n",Buf->front->next->PageID);
		Page *p_out = SearchPT(PT,Buf->front->next->PageID);//缓冲区队首的页 		
		
		//淘汰页调出缓冲区队列 
		PageNode *pn = Buf->front->next;
		//printf("表%s页%d调出缓冲区\n",TableList[pn->page->TableNo],pn->PageID); 
		Buf->front->next = pn->next;
		
		Buf->length -=1; //缓冲区长度减1 
		
		//-------------------------------------------- 
		//数据块写入文件 
		//---------------------------------------------	
		Block *b=pn->block;
		Block c=*b;
		SaveBlock(c);
		
		free(pn->block);
		pn->block = NULL; 
		free(pn);
		pn = NULL;
		//根据ID ,找到内存中的块 
		// 
		//
		
				
		UpdatePage(PT,p_out,0);
	}

	
	PageNode *PN = InitPN();
	PN->PageID = p_in->PageID;
	//页节点写入缓冲队列
	if(Buf->front == Buf->rear){
		//printf("缓冲区为空\n");
		
		Buf->front->next = PN;

		Buf->rear = PN;

	
	}
	else{
		Buf->rear->next = PN;
		Buf->rear = PN;
	}

	//printf("表%s页%d调入缓冲区\n",TableList[p_in->TableNo],p_in->PageID);
	
	UpdatePage(PT,p_in,1);	
	Buf->length +=1;
	
	//-------------------------------------------- 
	//数据块写入内存 
	//---------------------------------------------	
	//printf("%d\n",p_in->BlockID);
	//printf("%d\n",p_in->BlockID);
	Block b;
	if(p_in->CreateBlock)
		b = ReadBlock(p_in->TableNo,p_in->BlockID);
	else
		{
			Block *d=(Block *)malloc(sizeof(Block));
			d=NewBlock(p_in->TableNo,p_in->BlockID);
			b=*d;
			p_in->CreateBlock=true;
		}	
	//printf("%d\n",b.BlockNo);
	Block *c;
	c=(Block *)malloc(sizeof(Block));
	*c=b;
	PN->block=c;
	PN->page=p_in;
	
	//printf("%d\n",PN->block->BlockNo);
	//return 内存中的block 

	return c;

	//printBuf(Buf);
}

void clearBuf(BufferQue* &Buf)//退出数据库时，将缓冲区中的全部页写入文件 
{
	int l = Buf->BufferLength;
	PageNode *pn=Buf->front;
	for(int i=0;i<Buf->length;i++)
	{
		pn=pn->next;
		Block *b=pn->block;
		Block c=*b;
		SaveBlock(c);
		//printf("表%s页%d调写入文件\n",TableList[c.TableNo],c.BlockNo);
	 }
	Buf =  InitBuffer();
	Buf->BufferLength = l;
}



int UpdateRecord(int tableno,int blockno,int offset,char *InputRecord)//更新记录（感觉都需要传入指针，明日再改为指针，上面的可能都要改为指针）
{
	//先查看缓冲区内是否有此块
	Block cur_block=ReadBlock(tableno,blockno);//如果没有读出这个块则将其从文件中读出到cur_block
	//更新块信息
	memset(cur_block.Re[offset],0x00, sizeof (char) * 255);
	strcpy(cur_block.Re[offset],InputRecord);
	cur_block.LastUsedRecord=offset;
	SaveBlock(cur_block);//把块写回磁盘（写回缓冲区需修改）
	return 0;
}

int SavePageIndex(Page *P,int i,int TNo)
{
	 
	if(P->CreateBlock == false)
		return 0;

	FILE *f;
	char s[255]; 
    sprintf(s, "%s\\PageInfo\\%d",TableList[TNo],i);
    remove(s);
	sprintf(s, "%s\\PageInfo\\%d",TableList[TNo],i);
	f = fopen(s,"w+");
    fclose(f);
    f = fopen(s,"rb+");
	if(f != NULL)
	{ 
		fwrite(&(P->PageID), sizeof(P->PageID), 1, f);
        //fseek(f, sizeof(P->PageID), 0);
        fwrite(&(P->BlockID), sizeof(P->BlockID), 1, f);
        //fseek(f, sizeof(P->BlockID), 0);
        fwrite(&(P->CreateBlock), sizeof(P->CreateBlock), 1, f);
        //fseek(f, sizeof(P->CreateBlock), 0);
        fwrite(&(P->KeyList), sizeof(P->KeyList), 1, f);
        fclose(f);
        return 0;
    }
}
int SavePTIndex(PageTable *PT,int TNo)
{
	Page *P = PT->next;
	for(int i=0;i<PAGENUM;i++){
		SavePageIndex(P,i,TNo);
		P=P->next;	
	}
	return 0;
}
Page ReadPageIndex(int i,int TNo)//将一页的信息写入文件 
{
	Page P;
	Page *p=InitPage(i,TNo);
	P=*p;
	FILE *f;
	char s[255]; 
    sprintf(s, "%s\\PageInfo\\%d",TableList[TNo],i);
    //printf("%s\n",s);
    f = fopen(s, "r");//以rb+方式打开表二进制文件（需调整）
    if(f==NULL)
    {
	   return P;
	}
	if(f != NULL)
	{ 
        fread(&(P.PageID), sizeof(P.PageID), 1, f);
        //fseek(f, sizeof(P.PageID), 0);
        fread(&(P.BlockID), sizeof(P.BlockID), 1, f);
        //fseek(f, sizeof(P.BlockID), 0);
        fread(&(P.CreateBlock), sizeof(P.CreateBlock), 1, f);
        //fseek(f, sizeof(P.CreateBlock), 0);
        fread(&(P.KeyList), sizeof(P.KeyList), 1, f);
        fclose(f);
        return P;
    }
}
PageTable *ReadPTIndex(int TNo)
{
	PageTable* PT = (PageTable*)malloc(sizeof(PageTable));
	(PageTable*)memset(PT,0x00,sizeof(PageTable));
	PT->next = NULL;
	PT->TableNo = TNo;
	for(int i=PAGENUM-1;i>=0;i--){
		Page *p = (Page *)malloc(sizeof(Page));
		Page p_tmp=ReadPageIndex(i,TNo);
		*p=p_tmp;
		
		p->next = PT->next;
		PT->next = p;
	}
		
	return PT;	
 } 
