#pragma once 
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include"database.h"
//#include"BPlusTree.cpp"
/*
typedef structs
{
}Record;//��¼�ṹ�� 
*/
//#define TableNO 1


typedef struct 
{   
	int BlockNo;//���
	int TableNo;//�������
    int LastUsedRecord;//���һ����ʹ�õļ�¼
	bool RecordCondition[MAX_OF_RECORD];//��Ǽ�¼�Ƿ�ʹ�� ʹ��Ϊ1��δʹ��Ϊ0\
	//Record Re[MAX_OF_RECORD];//��¼
	char Re[MAX_OF_RECORD][255];
	int FirstAvailableBlock;//��һ�����õļ�¼�ռ�
}Block;//��Ľṹ�� 

typedef struct Page{//ҳ 
	int PageID;//ҳ�� 
	int TableNo;
	bool IFBuffer;//�Ƿ������� 
	int BlockID;//���
	bool CreateBlock;//�Ƿ񴴽��� 
	char KeyList[MAX_OF_RECORD][8];//�����б� 
	unsigned char F_Update;//�Ƿ���� 
	Page *next;
}Page;

typedef struct{//ҳ�� 
	Page *next;//��ҳ 
	int PageNum;//ҳ������
	int TableNo;//���
	//bool PT_Update;//�Ƿ���� 
}PageTable;

typedef struct PageNode{//��������ҳ�ڵ�
	int TableNo;//�ڵ��Ӧ�ı�� 
	int PageID;// �ڵ��Ӧ��ҳ��
	Page *page;//�ڵ��Ӧ��ҳ 
	Block *block; //�ڵ��Ӧ�Ŀ�
	PageNode *next; 
	
}PageNode;

typedef struct{//���������� 
	PageNode *front;//ָ��ͷ��� 
	PageNode *rear;//ָ���β 
	int BufferLength;//��������󳤶� 
	int length; //��ǰ���� 
}BufferQue;

void PrintBlock(Block *block) 
{
	printf("%d\n%d\n%d\n%d\n",block->BlockNo,block->FirstAvailableBlock,block->LastUsedRecord,block->TableNo);
	for (int i=0;i<MAX_OF_RECORD;i++)
	{
		printf("%s\n",block->Re[i]);
	}
} 
int SaveBlock(Block block)//�洢�飬��������ǿ�Ľṹ�壬���Զ������ļ�
{
    FILE *f;
    char filepath[255];
    sprintf(filepath,"%s//data",TableList[block.TableNo]);
    //printf("%d\n",block.BlockNo); 
    f = fopen(filepath, "rb+");//��rb+��ʽ�򿪱�������ļ���������ļ�����
    if(f==NULL)
    {
       f = fopen(filepath,"w+");
       fclose(f);
       f = fopen(filepath, "rb+");
	}
    if(f != NULL)
	{ 
		fseek(f, (block.BlockNo )* SIZE_OF_BLOCK, 0);//�ѿ�д���ļ��е�BlockNo��λ��
        fwrite(&block, sizeof(Block), 1, f);
        fclose(f);
        return 0;
    }
}

Block ReadBlock(int tablenumber,int blocknumber) //�ӵ�tableno�����е�blockno��������飬���ؿ�Ľṹ��,���ȴ����������ҵ���ı�źͿ��
{
	FILE *f;
	char filepath[255];
    sprintf(filepath,"%s//data",TableList[tablenumber]);
    f = fopen(filepath, "rb+");//��rb+��ʽ�򿪱�������ļ����������
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
Block * NewBlock(int tableNo,int blockNo)//�����¿飬��Ҫ���ڱ��п鶼����ʱ�����µĿ�ռ䣬���ؿ�ָ��
{
    /*if()
    {
       ���п������Ѵ����ʱ�����±��������� 
    }*/
	Block *newblock=(Block*) malloc(sizeof(Block));//�����ռ�
    newblock->BlockNo = blockNo;//����
    newblock->TableNo = tableNo;//���������
    newblock->LastUsedRecord = 0;//�����һ����ʹ�õļ�¼
    newblock->FirstAvailableBlock =MAX_OF_RECORD-1;
    int i = 0;
    for(i = 0; i < MAX_OF_RECORD; i++)
    {
        newblock->RecordCondition[i]=0;
    }
    //����ͷ��Ϣд����ռ��ͷ��
    return newblock;
}

int IfBlockIsFull(Block *block)//�ж��Ƿ��������,��������򷵻�1����δ���򷵻�0
{
	if(block->FirstAvailableBlock==-1) return 1;
	else return 0;
}

Page* InitPage(int i,int TNo)//ҳ��ʼ�� 
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

PageTable* InitPT(int TNo){//ҳ���ʼ�� 
	PageTable* PT = (PageTable*)malloc(sizeof(PageTable));
	(PageTable*)memset(PT,0x00,sizeof(PageTable));
	PT->TableNo = TNo;
	//������������������������ҳ������˳�� 
	for(int i=PAGENUM-1;i>=0;i--){
		Page *p = InitPage(i,TNo);
		p->next = PT->next;
		PT->next = p;
		//printf("%d",i);
	}
		
	return PT;
}
PageNode *InitPN(){//������ҳ�ڵ��ʼ�� 
	PageNode* PN = (PageNode*)malloc(sizeof(PageNode));
	(PageNode*)memset(PN,0x00,sizeof(PageNode));
	PN->PageID = -1;
	PN->page = NULL;
	PN->block = NULL;////////////////////////////////
	PN->next = NULL;
	return PN; 
}

BufferQue* InitBuffer(){//��������ʼ�� 
	BufferQue* BQ = (BufferQue*)malloc(sizeof(BufferQue));
	(BufferQue*)memset(BQ,0x00,sizeof(BufferQue));

	PageNode *PN = InitPN();
	BQ->front = PN;
	BQ->rear = BQ->front;
	BQ->BufferLength = BufLength;
	BQ->length = 0;
	return BQ; 
}
void printBuf(BufferQue *Buf){//��ӡ��������Ϣ 
	printf("��ǰ���������С�");
	PageNode *pn = Buf->front->next;
	for(int i = 0;i<Buf->length;i++)
	{
		printf("%d ",pn->PageID);
		pn = pn->next;
	}
	printf("��\n") ;
	
}
Page *SearchPT(PageTable *PT,int ID){//��ҳ�����ҵ�ָ��ҳ 
	Page *p = PT->next;
	while(p){
		if(p->PageID == ID){
			//printf("��ҳ�����ҵ�ҳ%d\n",ID);
			return p;
		}
		p = p->next;
	}
}
void UpdatePage(PageTable *&PT,Page *&p,int option){//1:д���ڴ� 0��д���ڴ�
	
	if(option == 1){//
		p->IFBuffer = true;
	} 
	else if(option == 0){
		p->IFBuffer = false;
		p->F_Update = 0;
	}
}
Block* FIFO(PageTable *&PT,BufferQue* &Buf,Page *&p_in){//�������������ҳ,����ҳ��Ӧ��block
	//p_in->CreateBlock = true;
	/*if(Buf->BufferLength == 1){
		Buf->front->next = p_in;
	}*/
	if(Buf->length == Buf->BufferLength){//�������������� 
		printf("��������������\n");
		printf("%d\n",Buf->front->next->PageID);
		Page *p_out = SearchPT(PT,Buf->front->next->PageID);//���������׵�ҳ 		
		
		//��̭ҳ�������������� 
		PageNode *pn = Buf->front->next;
		printf("��%sҳ%d����������\n",TableList[pn->page->TableNo],pn->PageID); 
		Buf->front->next = pn->next;
		
		Buf->length -=1; //���������ȼ�1 
		
		//-------------------------------------------- 
		//���ݿ�д���ļ� 
		//---------------------------------------------	
		Block *b=pn->block;
		Block c=*b;
		SaveBlock(c);
		
		free(pn->block);
		pn->block = NULL; 
		free(pn);
		pn = NULL;
		//����ID ,�ҵ��ڴ��еĿ� 
		// 
		//
		
				
		UpdatePage(PT,p_out,0);
	}

	
	PageNode *PN = InitPN();
	PN->PageID = p_in->PageID;
	//ҳ�ڵ�д�뻺�����
	if(Buf->front == Buf->rear){
		printf("������Ϊ��\n");
		
		Buf->front->next = PN;

		Buf->rear = PN;

	
	}
	else{
		Buf->rear->next = PN;
		Buf->rear = PN;
	}

	printf("��%sҳ%d���뻺����\n",TableList[p_in->TableNo],p_in->PageID);
	
	UpdatePage(PT,p_in,1);	
	Buf->length +=1;
	
	//-------------------------------------------- 
	//���ݿ�д���ڴ� 
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
	//return �ڴ��е�block 

	return c;

	//printBuf(Buf);
}

void clearBuf(BufferQue* &Buf)//�˳����ݿ�ʱ�����������е�ȫ��ҳд���ļ� 
{
	int l = Buf->BufferLength;
	PageNode *pn=Buf->front;
	for(int i=0;i<Buf->length;i++)
	{
		pn=pn->next;
		Block *b=pn->block;
		Block c=*b;
		SaveBlock(c);
		printf("��%sҳ%d��д���ļ�\n",TableList[c.TableNo],c.BlockNo);
	 }
	Buf =  InitBuffer();
	Buf->BufferLength = l;
}



int UpdateRecord(int tableno,int blockno,int offset,char *InputRecord)//���¼�¼���о�����Ҫ����ָ�룬�����ٸ�Ϊָ�룬����Ŀ��ܶ�Ҫ��Ϊָ�룩
{
	//�Ȳ鿴���������Ƿ��д˿�
	Block cur_block=ReadBlock(tableno,blockno);//���û�ж��������������ļ��ж�����cur_block
	//���¿���Ϣ
	memset(cur_block.Re[offset],0x00, sizeof (char) * 255);
	strcpy(cur_block.Re[offset],InputRecord);
	cur_block.LastUsedRecord=offset;
	SaveBlock(cur_block);//�ѿ�д�ش��̣�д�ػ��������޸ģ�
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
Page ReadPageIndex(int i,int TNo)//��һҳ����Ϣд���ļ� 
{
	Page P;
	Page *p=InitPage(i,TNo);
	P=*p;
	FILE *f;
	char s[255]; 
    sprintf(s, "%s\\PageInfo\\%d",TableList[TNo],i);
    //printf("%s\n",s);
    f = fopen(s, "r");//��rb+��ʽ�򿪱�������ļ����������
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
