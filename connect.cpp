#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include <unistd.h>
//PageTable *PTList[MAX_OF_TABLE];//全局页表
//BufferQue *Buf;//全局缓冲区
//BPlusTree BTList[MAX_OF_TABLE];//全局B+树

PageNode* connectBuf_Block(BufferQue* Buf,int Bno,int TNo)//缓冲区找到块Bno 
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

void KaConnect(int TNo1,int TNo2,BufferQue* &Buf1,PageTable *PT1,PageTable *PT2)//卡式积连接
{
	printf("清空缓冲区\n");
   	clearBuf(Buf1);//清空缓冲区
   	printf("缓冲区清空成功\n");
   	//printf("%d\n",Buf1->length);
   		
   	Page *P1 = PT1->next;
   	while(P1)
	{
   		if(strlen(P1->KeyList[MAX_OF_RECORD-1]) !=0){
   			//printf("0\n");
   			FIFO(PT1,Buf1,P1);//将表TNo1的块全部读入内存Buf1 
		}			   
		P1 = P1->next;
	}
	printf("表%s已全部读入缓冲区\n",TableList[TNo1]);	
	BufferQue* Buf2 = InitBuffer();
	Buf2->BufferLength = 1;
	printf("缓冲区2创建成功\n"); 
	Page *P2 = PT2->next;
	Record *R1= SetRecord(TNo1);
	Record *R2= SetRecord(TNo2);
	Record *R=InitRecord();
	char tab1[5],tab2[5];
	sprintf(tab1,"%.4d",TNo1);
	sprintf(tab2,"%.4d",TNo2);
	R=recjoin(tab1,tab2);
	strcpy(R->Head->tablenum,"9999");
	int sum = 0;
	while(P2)
	{
		if(strlen(P2->KeyList[MAX_OF_RECORD-1]) !=0)
		{
			Block *b = FIFO(PT2,Buf2,P2);//将表TNo2的块读入内存Buf2				
			for(int t = MAX_OF_RECORD-1;t>=0 ;t--)
			{
				if(strlen(P2->KeyList[t]) == 0)
						break;
				R2 = output(b->Re[t]);
				PageNode* pn1 = Buf1->front->next;				
				while(pn1){
					for(int tmp = MAX_OF_RECORD-1;tmp>=0;tmp--){
						if(strlen(pn1->page->KeyList[tmp]) == 0)
							break;
						R1 = output(pn1->block->Re[tmp]);
						R = Recjoin(R1,R2,R,sum);
						printf("%s\n",input(R)+8);
					}
					pn1 = pn1->next;
				}
			}
			clearBuf(Buf2);	
		}			   
		P2 = P2->next;
	}
} 
//表连接单条件 
void ConnectTable1(int TNo1,int TNo2,int ANo1,int ANo2,int Option,BufferQue* &Buf1,PageTable *PT1,PageTable *PT2)//Option:1-等值 0-不等值  
{
	//为TNo1表的ANo1属性建立索引
	CharBPlusTree CBT = InitializeCharTree();
	createCharTree(CBT,PT1,Buf1,TNo1,ANo1);
	printf("索引创建成功\n");
	//TravelCharTree(CBT);
	
	//将TNo1的块全部写入缓冲区Buf1
	//printf("清空缓冲区\n");
   	clearBuf(Buf1);//清空缓冲区
   	//printf("缓冲区清空成功\n");
   	//printf("%d\n",Buf1->length);   		
   	Page *P1 = PT1->next;
   	while(P1)
	{
   		if(strlen(P1->KeyList[MAX_OF_RECORD-1]) !=0){
   			FIFO(PT1,Buf1,P1);//将表TNo1的块全部读入内存Buf1 
		}
		P1 = P1->next;
	}		
	printf("表%s已全部读入缓冲区\n",TableList[TNo1]);		
		
	BufferQue* Buf2 = InitBuffer();
	Buf2->BufferLength = 1;
	printf("缓冲区2创建成功\n"); 
	Page *P2 = PT2->next;
	Record *R1= SetRecord(TNo1);
	Record *R2= SetRecord(TNo2);
	Record *R=InitRecord();
	char tab1[5],tab2[5];
	sprintf(tab1,"%.4d",TNo1);
	sprintf(tab2,"%.4d",TNo2);
	R=recjoin(tab1,tab2);
	strcpy(R->Head->tablenum,"9999");
	for(int index=0;index<R->Head->attrubuteNum;index++){
		printf("%s   ",R->attribute[index]->name);
	}
	printf("\n");
	int sum = 0;
	while(P2)
	{
		if(strlen(P2->KeyList[MAX_OF_RECORD-1]) !=0)
		{
			Block *b = FIFO(PT2,Buf2,P2);//将表TNo2的块读入内存Buf2			
			for(int t = MAX_OF_RECORD-1;t>=0 ;t--)
			{
				if(strlen(P2->KeyList[t]) == 0)
					break;
				R2 = output(b->Re[t]);
				if(Option == 1){
					int ofset;
					//printf("%s\n",R2->attribute[ANo2]->value);
					//TravelCharTree(CBT);
					CharBPlusTree T = search(CBT,R2->attribute[ANo2]->value,ofset);
					if(T == NULL){
						//printf("!!");
						break;
					}
						
					
					int i = ofset;
					//TravelCharTree(CBT);
					while(1)
					{
						if(strcmp(T->Key[i],"-111111111")==0||i==M)	
						{
							T=T->Next;
							if(T == NULL)
								break;
							i=0;
							continue;
						}
						else
						{
							if(strcmp(R2->attribute[ANo2]->value,T->Key[i])!=0)
							{
								break;
								
							}							
							PageNode* pn1 = connectBuf_Block(Buf1,T->blockno[i],TNo1);
							//printf("%d\n",i);
							R1 = output(pn1->block->Re[T->offset[i]]);
							R = Recjoin(R1,R2,R,sum);
							printf("%s\n",input(R)+8);
							free(R1);
							
						//	printf("%s %d\n",T->Key[i],i);
						}
						i++;
					}		
				}/*
				else if(Option == 0){
					PageNode* pn1 = Buf1->front->next;				
					while(pn1){
						for(int tmp = MAX_OF_RECORD-1;tmp>=0;tmp--){
							if(strlen(pn1->page->KeyList[tmp]) == 0)
								break;
							char s1[500];
							sprintf(s1,"%.4d%s",TNo1,pn1->block->Re[tmp]);
							R1 = output(s1);
							if(strcmp(R1->attribute[ANo1]->value,R2->attribute[ANo2]->value)==0)
								continue;
							R = Recjoin(R1,R2,R);
							printf("%s\n",input(R)+8);
						}
						pn1 = pn1->next;
					}
				}*/	
					//b->Re[atoi(k)]
			}
			clearBuf(Buf2);
		}	   
		P2 = P2->next; 		
	}
}
//表连接多条件
void ConnectTable2(int TNo1,int TNo2,int ANo1,int ANo2,BufferQue* &Buf1,PageTable *PT1,PageTable *PT2,int ANo3,int ANo4)//  
{
	//为TNo1表的ANo1属性建立索引
	CharBPlusTree CBT = InitializeCharTree();
	createCharTree(CBT,PT1,Buf1,TNo1,ANo1);
	printf("索引创建成功\n");
	//TravelCharTree(CBT);
	
	//将TNo1的块全部写入缓冲区Buf1
	//printf("清空缓冲区\n");
   	clearBuf(Buf1);//清空缓冲区
   	//printf("缓冲区清空成功\n");
   	//printf("%d\n",Buf1->length);   		
   	Page *P1 = PT1->next;
   	while(P1)
	{
   		if(strlen(P1->KeyList[MAX_OF_RECORD-1]) !=0){
   			FIFO(PT1,Buf1,P1);//将表TNo1的块全部读入内存Buf1 
		}
		P1 = P1->next;
	}		
	printf("表%s已全部读入缓冲区\n",TableList[TNo1]);		
		
	BufferQue* Buf2 = InitBuffer();
	Buf2->BufferLength = 1;
	printf("缓冲区2创建成功\n"); 
	Page *P2 = PT2->next;
	Record *R1= SetRecord(TNo1);
	Record *R2= SetRecord(TNo2);
	Record *R=InitRecord();
	char tab1[5],tab2[5];
	sprintf(tab1,"%.4d",TNo1);
	sprintf(tab2,"%.4d",TNo2);
	R=recjoin(tab1,tab2);
	strcpy(R->Head->tablenum,"9999");
	int sum = 0;
	while(P2)
	{
		if(strlen(P2->KeyList[MAX_OF_RECORD-1]) !=0)
		{
			Block *b = FIFO(PT2,Buf2,P2);//将表TNo2的块读入内存Buf2			
			for(int t = MAX_OF_RECORD-1;t>=0 ;t--)
			{
				if(strlen(P2->KeyList[t]) == 0)
					break;
				R2 = output(b->Re[t]);
				int ofset;

				CharBPlusTree T = search(CBT,R2->attribute[ANo2]->value,ofset);
					if(T == NULL)
						continue;
					
					int i = ofset;
					//TravelCharTree(CBT);
					while(1)
					{
						if(strcmp(T->Key[i],"-111111111")==0||i==M)	
						{
							T=T->Next;
							if(T == NULL)
								break;
							i=0;
							continue;
						}
						else
						{
							if(strcmp(R2->attribute[ANo2]->value,T->Key[i])!=0)
							{
								break;
								
							}							
							PageNode* pn1 = connectBuf_Block(Buf1,T->blockno[i],TNo1);
							char s1[500];
							sprintf(s1,"%.4d%s",TNo1,pn1->block->Re[T->offset[i]]);
							//printf("%d\n",i);
							R1 = output(s1);
							
							if(strcmp(R1->attribute[ANo3]->value,R2->attribute[ANo4]->value)==0)//不满足第二个条件
							{
								R = Recjoin(R1,R2,R,sum);
								printf("%s\n",input(R)+8);
							}
							
						
						//	printf("%s %d\n",T->Key[i],i);
						}
						i++;
					}
			}
			clearBuf(Buf2);
		}	   
		P2 = P2->next; 		
	}
}
void ConnectOperate(PageTable **PTList,BufferQue *&Buf,int sign)
{
	printf("请输入第一个条件:\n");
	char input[500] = "";
	char TName1[20],TName2[20],AName1[20],AName2[20];
	
    scanf("%s",&input);
    //gets(input);
    //getchar();
    char *token = strtok(input,".");

    sprintf(TName1,"%s",token);

    token = strtok( NULL, "=" );
    sprintf(AName1,"%s",token );
    
	//printf("%c\n",AName1[strlen(AName1)-1]); 
	int Option = (AName1[strlen(AName1)-1]!='!');
	if(!Option){
		//printf("!!!!\n");
		AName1[strlen(AName1)-1]=0;
	}
	printf("%d\n",Option);
    token = strtok( NULL, "." );
	sprintf(TName2,"%s",token);
	token = strtok( NULL, "." );

    sprintf(AName2,"%s",token);
    //printf("%s\n%s\n%s\n%s\n",TName1,AName1,TName2,AName2);
    if(access(TName1,F_OK) == -1){
    	printf("表%s不存在\n",TName1);
    	return;
	}
	if(access(TName2,F_OK) == -1){
    	printf("表%s不存在\n",TName2);
    	return;
	}
	printf("找到表%s\n",TName1);
	int TNo1 = -1;
	//找到对应表号 
	for(int i = 0;i<MAX_OF_TABLE;i++){
		if(strcmp(TName1,TableList[i]) == 0){
			TNo1 = i;
			break;
		}
	}
	//printf("%d\n",TNo1);
	int TNo2 = -1;
	//找到对应表号 
	for(int i = 0;i<MAX_OF_TABLE;i++){
		if(strcmp(TName2,TableList[i]) == 0){
			TNo2 = i;
			break;
		}
	}
	int ANo1,ANo2;
	ANo1=atoi(AName1);
	ANo2=atoi(AName2);
	if(sign == 2){
		printf("请输入第二个条件:\n");
		char input2[500] = "";
		char AName3[20],AName4[20];
		
	    scanf("%s",&input2);
	    //gets(input);
	    //getchar();
	    char *token = strtok(input2,".");
	
	    //sprintf(TName1,"%s",token);
	
	    token = strtok( NULL, "=" );
	    sprintf(AName3,"%s",token );
	    token = strtok( NULL, "." );
		//sprintf(TName2,"%s",token);
		token = strtok( NULL, "." );
	
	    sprintf(AName4,"%s",token);
		int ANo3,ANo4;
		ANo3 = atoi(AName3);
		ANo4 = atoi(AName4);
		ConnectTable2(TNo1,TNo2,ANo1,ANo2,Buf,PTList[TNo1],PTList[TNo2],ANo3,ANo4);
		return;
		
	}
	//Record *R1= SetRecord(TNo1);
	//printf("%s\n",R1->attribute[1]->type);
	ConnectTable1(TNo1,TNo2,ANo1,ANo2,Option,Buf,PTList[TNo1],PTList[TNo2]);
	return;
	//Record *R2= SetRecord(TNo2);
	//printf("%d\n",TNo2);
    //NATION.1=NATION.1
    //NATION.2=NATION.d
    /*
    		{
    			
				getchar();
				*/
} 
/*
int main(){
	//FILE* bufLog=fopen("log/buflog","a+");//缓冲区日志 
    //FILE* pageLog=fopen("log/pagelog","a+");//页日志 
    //FILE* blockLog=fopen("log/blocklog","a+");//数据块日志 
    
	ConnectOperate();

	int i;
	printf("缓冲区初始化\n"); 
	Buf = InitBuffer();//缓冲区初始化
	printf("缓冲区初始化成功\n");
	//页表、B+树初始化 
	printf("段页表初始化\n");
	for(i = 0;i < MAX_OF_TABLE;i++){
		int TNo = i;
		if(access(TableList[TNo],F_OK) == -1)
		{
			PTList[TNo] = NULL;
		}	
		else{			
			PTList[TNo] = ReadPTIndex(TNo);
		}
	}
	printf("段页表初始化成功\n");
	int TNo1=0;
	int TNo2=0;
	initrecinfo();
	ConnectTable1(TNo1,TNo2,0,0,1,Buf,PTList[TNo1],PTList[TNo2]);
	//ConnectTable2(TNo1,TNo2,0,0,Buf,PTList[TNo1],PTList[TNo2],1,1);
	//KaConnect(TNo1,TNo2,0,0,Buf,PTList[TNo1],PTList[TNo2]);
	
	

	printf("成功退出数据库\n");
	
    return 0;
}
*/
