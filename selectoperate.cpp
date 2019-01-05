#pragma once 
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include"page.cpp"
#include"index1.cpp"

int getContidion(char* condition){
    if(strlen(condition)==1){
        if(condition[0]=='=')//���� 
            return 1;
        if(condition[0]=='>')//���� 
            return 2;
        if(condition[0]=='<')//С�� 
            return 3;
    }
    else{
        if(condition[1]=='>'&&condition[0]=='<')//������ 
            return -1;
        if(condition[0]=='>')//���ڵ��� 
            return -2;
        if(condition[0]=='<')//С�ڵ��� 
            return -3;
    }
}
void SelectInt(BPlusTree T,PageTable *&PT,BufferQue *&Buf,int TNo,char *attr,int operate,char *val,char attrname[Max_Attribute_Num][20],int sum){
	int temp=atoi(val);
	int i=0;
	//max=FindMax(BT);
	if(operate==1){//���� 
//		printf("!!!\n"); 
		SEARCHINT(T,temp,0,PT,Buf,attrname,sum);
	}
	if(operate==2){//���� 
		SEARCHINT1(T,temp,PT,Buf,attrname,sum);	 
	}
	if(operate==3){//С�� 
		SEARCHINT2(T,temp,PT,Buf,attrname,sum);
	}
	if(operate==-1){//������ 
		SEARCHINT3(T,temp,PT,Buf,attrname,sum);		
	}
	if(operate==-2){//���ڵ��� 
		
		SEARCHINT(T,temp,0,PT,Buf,attrname,sum);
		SEARCHINT1(T,temp,PT,Buf,attrname,sum);
	}
	if(operate==-3){//С�ڵ��� 
		SEARCHINT2(T,temp,PT,Buf,attrname,sum);
		SEARCHINT(T,temp,0,PT,Buf,attrname,sum);
	}
    
}
void SelectChar(CharBPlusTree T,PageTable *&PT,BufferQue *&Buf,int TNo,char *attr,int operate,char *val,char attrname[Max_Attribute_Num][20],int sum)
{             //CharBPlusTree T,PageTable *&PT,BufferQue *&Buf,int TNo,char attr[50],int operate,char val[300] 	
	if(operate==1){//���� 
//		printf("!!!\n"); 
		SEARCH(T,val,0,PT,Buf,attrname,sum);
	}
	if(operate==2){//���� 
		SEARCH1(T,val,PT,Buf,attrname,sum);	 
	}
	if(operate==3){//С�� 
		SEARCH2(T,val,PT,Buf,attrname,sum);
	}
	if(operate==-1){//������ 
		SEARCH3(T,val,PT,Buf,attrname,sum);		
	}
	if(operate==-2){//���ڵ��� 
		
		SEARCH(T,val,0,PT,Buf,attrname,sum);
		SEARCH1(T,val,PT,Buf,attrname,sum);
	}
	if(operate==-3){//С�ڵ��� 
		SEARCH2(T,val,PT,Buf,attrname,sum);
		SEARCH(T,val,0,PT,Buf,attrname,sum);
	}
}

void SelectWithOneCondition(PageTable *&PT,BufferQue *&Buf,int TNo,char *attr,char *op,char *val,char attrname[Max_Attribute_Num][20],int sum)
{
	char enter = '0';
	int i,j,k=0,ANo,s;
//	printf("11111\n");
	int operate=getContidion(op);
//	printf("%d\n",operate);
	//if attr=int
	if(atoi(val)){
		BPlusTree BT = Initialize();
	//	printf("111111\n");
		for(int i=0;i<Recinfo[TNo]->Head->attrubuteNum;i++)
		{
			if(strcmp(attr,Recinfo[TNo]->attribute[i]->name)==0)
			{
				ANo=i;
				break;
			}
		}
	    createIntTree(BT,PT,Buf,TNo,ANo);
	//	Travel(BT);	
	//	printf("build int tree\n");
		SelectInt(BT,PT,Buf,TNo,attr,operate,val,attrname,sum);
	}
	else{
//			printf("22222\n");
//			printf("build char tree\n");
			CharBPlusTree T = InitializeCharTree();
			for(int i=0;i<Recinfo[TNo]->Head->attrubuteNum;i++)
			{
				if(strcmp(attr,Recinfo[TNo]->attribute[i]->name)==0)
				{
					ANo=i;
					break;
				}
			}
			createCharTree(T,PT,Buf,TNo,ANo);
//			printf("33333\n");
//			TravelCharTree(T);
		//	printf("%d\n",operate);
			SelectChar(T,PT,Buf,TNo,attr,operate,val,attrname,sum);
	}
		
    //SelectOperate(BT,PT,Buf,attr,con,val,TNo);
//	printf("%s\n",SelectRecord(BT,PT,Buf,val,TNo));
} 
void SelectWithTwoCondition(BPlusTree &BT,PageTable *&PT,BufferQue *&Buf,int TNo,char condition1[500],char condition2[500])
{
	char enter = '0';
	char attr[50]={0};
	char op[4]={0};
	char val[300]={0};
	int i,j,k=0;
	printf("�ڱ�%s�в�������Ϊ%s��%s�ļ�¼\n",TableList[TNo],condition1,condition2);
	while(condition1[i]>='A'&&condition1[i]<='z')//ʶ������ 
	{
		attr[i]=condition1[i];
		i++;
	}	
	op[0]=condition1[i];  //ʶ������� 
	i++;
	if(condition1[i]>'9'&&condition1[i]<'A')
	{
		op[1]=condition1[i++];
		op[2]='\0';
	}
	else{
		op[1]='\0';
	}
	for(;i<strlen(condition1);i++,k++)  //ʶ��ֵ 
	{
		val[k]=condition1[i];
	}
	

    //SelectOperate(BT,PT,Buf,attr,con,val,TNo);
	//printf("%s\n",SelectRecord(BT,PT,Buf,val,TNo));
} 

