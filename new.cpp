#include<stdio.h>
#include <io.h>
#include <direct.h>
#include"database.cpp"

//PageTable *PTList[MAX_OF_TABLE];//ȫ��ҳ��
//BufferQue *Buf;//ȫ�ֻ�����
//BPlusTree BTList[MAX_OF_TABLE];//ȫ��B+��
int Get_TNo(char *TName)//���ݱ����ҵ���� 
{
	int i;
	for(i=0;i<MAX_OF_TABLE;i++){
		if(strcmp(TableList[i],TName)==0)
			break;
	}
	return i;	
}
int CREATE_TABLE(char *TName,int ANum,Record *R)
{
	if(access(TName,F_OK)==0)//�����ļ��У����иñ�����ʧ�� 
		return 2;
	mkdir(TName);
	FILE *f1;
	char s1[255];
	int TNo = Get_TNo(TName);
	sprintf(s1, "%.4d",TNo);
    f1 = fopen(s1, "w+");//��ֻд��ʽ���ļ�,�ļ�������ɾ�� 
    printf("��ʼ������%s\n",TName);
    fprintf(f1,"%d\n",ANum);
    for(int i=0;i<ANum;i++)
		fprintf(f1,"%d %d %.4d %s\n",R->attribute[i]->type,1,R->attribute[i]->AttributeLength,R->attribute[i]->name);
	printf("��ʼ������%s\n",TName);
	char p1[255];
	sprintf(p1,"%s\\PageInfo", TName);
	mkdir(p1);
	char p2[255];
	sprintf(p2,"%s\\BPlusTree", TName);
	mkdir(p2); 
	printf("��%s�����ɹ�\n", TName);
	fclose(f1);
	
	initrecinfo();
	//Recinfo[TNo] = readtable(s1);
	PTList[TNo] = ReadPTIndex(TNo);//ҳ���ʼ��
	BTList[TNo] = Initialize();//B+����ʼ�� 
	
	return 1;
}
void DBInit()//���ݿ��ʼ�� 
{
	//��������ʼ��
	printf("��������ʼ��\n"); 
	Buf = InitBuffer();//��������ʼ��
	printf("��������ʼ���ɹ�\n");
	
	//��ʼ����ҳ��
	printf("��ҳ���ʼ��\n");
	for(int i = 0;i < MAX_OF_TABLE;i++){
		PTList[i] = NULL;
	}
	printf("��ҳ���ʼ���ɹ�\n");
	
	//��ʼ��B+�� 
	printf("B+����ʼ��\n");
	for(int i = 0;i < MAX_OF_TABLE;i++){
			BTList[i] = NULL;
	}	
	printf("B+����ʼ���ɹ�\n");
	initrecinfo();
}
void REMOVE_P(char *path,int PNo)
{
	char filename[50];
	sprintf(filename,"%s\\%d",path,PNo);
	remove(filename);
	return;
}
char *REMOVE_PT(char *TName)
{
	char path[50];
	sprintf(path,"%s\\PageInfo",TName);
	for(int i = 0;i<PAGENUM;i++)
		REMOVE_P(path,i);
	return path;
}
void REMOVE_LEVEL0(char *path,int i)
{
	char filename[50];
	sprintf(filename,"%s\\%d",path,i);
	remove(filename);
	return;
}
void REMOVE_LEVEL1(char *path,int i,int j)
{
	char filename[50];
	sprintf(filename,"%s\\%d_%d",path,i,j);
	remove(filename);
	return;
}
void REMOVE_LEVEL2(char *path,int i,int j,int k)
{
	char filename[50];
	sprintf(filename,"%s\\%d_%d_%d",path,i,j,k);
	remove(filename);
	return;
}
char *REMOVE_BT(char *TName)
{
	char path[50];
	sprintf(path,"%s\\BPlusTree",TName);
	for(int i=0;i<M;i++){
		REMOVE_LEVEL0(path,i);
		for(int j=0;j<M;j++){
			REMOVE_LEVEL1(path,i,j);
			for(int k=0;k<M;k++)
				REMOVE_LEVEL2(path,i,j,k);
		}
	}
	return path;
}
//void REMOVE_BT()
int DROP_TABLE(char *TName)
{
	if(access(TName,F_OK)==-1)//�����ڴ����ļ��У������ڸñ�ɾ��ʧ��ʧ�� 
		return 2;
	int TNo = Get_TNo(TName);
	//��ջ�����
	printf("��ջ�����\n");
   	clearBuf(Buf);
   	printf("��������ճɹ�\n");
	
	
	printf("ɾ���ڴ��ж�ҳ��\n");
	if(PTList[TNo] != NULL)
	{
		PTList[TNo] = NULL;
	}
	printf("�ڴ��ж�ҳ��ɾ���ɹ�\n");
	
	printf("ɾ���ڴ���B+��\n");
	if(BTList[TNo] != NULL)
	{
		BTList[TNo] = NULL;
	}
	printf("�ڴ���B+��ɾ���ɹ�\n");
	
	printf("ɾ����%sĿ¼\n",TName);
	
	REMOVE_PT(TName);
	char path1[50];
	sprintf(path1,"%s\\PageInfo",TName);
	rmdir(path1);
	
	REMOVE_BT(TName);
	char path2[50];
	sprintf(path2,"%s\\BPlusTree",TName);
	rmdir(path2);
	
	char d[50];
	sprintf(d,"%s//data",TName);
	remove(d);//ɾ��data�ļ� 
	char s[50];
	sprintf(s, "%.4d",TNo);
	remove(s);
	rmdir(TName);
	printf("��%sĿ¼ɾ���ɹ�\n",TName);
	return 1;
}

void DBExit()
{
	printf("��ջ�����\n");
   	clearBuf(Buf);
   	printf("��������ճɹ�\n");
   	
   	printf("�����ҳ��\n");
   	for(int i=0;i < MAX_OF_TABLE;i++){   				
		if(PTList[i] != NULL)
			SavePTIndex(PTList[i],i);
	}
	printf("��ҳ����ɹ�\n");

	printf("����B+��\n");
   	for(int i=0;i<MAX_OF_TABLE;i++){
   		if(BTList[i] != NULL)
			SaveBPlusTree(BTList[i],i);
	}
	printf("B+������ɹ�\n"); 
}
int CHECK_TABLE(char *TName)
{
	if(access(TName,F_OK) == -1){
		printf("��%s������\n",TName);
		return 0;
	}			
	else{
		printf("�ҵ���%s\n",TName);
		int TNo = Get_TNo(TName);
		if(PTList[TNo] == NULL){
			PTList[TNo] = ReadPTIndex(TNo);//ҳ����� 
		    BTList[TNo] = ReadBPlusTree(TNo);//B+������ 
		}
		return 1; 
	}
}
int LOAD_TABLE(char *TName,char *filename)
{
	CHECK_TABLE(TName);
	int TNo = Get_TNo(TName);
	readFileData(BTList[TNo],PTList[TNo],Buf,filename,TNo);
	return 0;
}
int INSERT_RECORD(char *TName,char fieldname[Max_Attribute_Num][20],int fienum,char value[MAX_OF_ATTRIBUTE][255],int valnum)
{
  //  int slength = 0;
  	//printf("!!!!!!!\n");
  	CHECK_TABLE(TName);
	int TNo = Get_TNo(TName);
	
	if(fienum == 0){//����������¼ 
		if(valnum != Recinfo[TNo]->Head->attrubuteNum){
			printf("the record you want to insert into table %s does not have the right attribute number!\n",TName);
			return 0;
		}
		char **val = new char*[Max_Attribute_Num];
    	for (int i =0; i < valnum; ++i){
    		val[i] = new char[255];
    		strcpy(val[i],value[i]);
		} 	
		addRecord(BTList[TNo],PTList[TNo],Buf,value[0],val,TNo);
	}
	else{
		int *c;
		int ANo;
		int bb[Max_Attribute_Num];
		for(int i = 0;i<Max_Attribute_Num;i++)
		{ 
			bb[i] = 0;
		}
		c = bb;
		for(int i = 0;i<fienum;i++)
		{
			for(int j=0;j<Recinfo[TNo]->Head->attrubuteNum;j++)
			{
				if(strcmp(fieldname[i],Recinfo[TNo]->attribute[j]->name) == 0)
				{
					ANo=j;
					break;
				}
			}
	//		printf("%d\n",ANo);
			c=AddAttribute(c,ANo);
		}
		if(c[0] == 0){
			printf("the key attribute of your input is null!\n");
			return 0;
		}
		if(valnum > Recinfo[TNo]->Head->attrubuteNum){
			printf("the record you want to insert into table %s does not have the right attribute number!\n",TName);
			return 0;
		}
		char **val = new char*[Max_Attribute_Num];
    	for (int i =0; i < Recinfo[TNo]->Head->attrubuteNum; ++i){
    		val[i] = new char[255];
    		if(c[i] == 1){
    			strcpy(val[i],value[i]);
    			//printf("%s\n",val[i]);
			}
    		else{
    			//printf("%d________\n",i);
    			strcpy(val[i]," ");
    			//val[i] = NULL;
			}
		}
		//printf("!!!!!!!!!!!\n"); 	
		addRecord(BTList[TNo],PTList[TNo],Buf,val[0],val,TNo);
	}
	printf("success to insert record!\n");
	return 0;
 	
}		
void select_attr(char *TName,char attrname[Max_Attribute_Num][20],int sum,char *attr,char *op,char *val)
{
	CHECK_TABLE(TName);
	int TNo = Get_TNo(TName);
//	printf("%s %s %s\n",attr,op,val);
	SelectWithOneCondition(PTList[TNo],Buf,TNo,attr,op,val,attrname,sum);
	return;
}
void print_attr(char *TName,char attrname[Max_Attribute_Num][20],int sum)
{
	CHECK_TABLE(TName);
	int TNo = Get_TNo(TName);
	char *projectionrecord;
    projectionrecord=(char *)malloc(255*sizeof(char));
	int bb[Max_Attribute_Num];
	int *c;
	int ANo;
	for(int i=0;i<Max_Attribute_Num;i++)
	{ 
		bb[i]=0;
	}
	c=bb;
	int k; 
//	printf("1111\n");
	for(int i=0;i<sum;i++)
	{
			for(int j=0;j<Recinfo[TNo]->Head->attrubuteNum;j++)
		{
			if(strcmp(attrname[i],Recinfo[TNo]->attribute[j]->name)==0)
			{
				ANo=j;
				break;
			}
		}
	//		printf("%d\n",ANo);
			c=AddAttribute(c,ANo);
	}
//	
	clearBuf(Buf);//��ջ�����
	Page *P = PTList[TNo]->next;//ҳ������ҳ 
	Record *R= SetRecord(TNo);
	while(P){
		if(strlen(P->KeyList[MAX_OF_RECORD-1]) != 0)
		{
			Block *b=FIFO(PTList[TNo],Buf,P);
			//printf("1111\n");
			//
			for(int i = MAX_OF_RECORD-1;i>=0;i--)
			{
				if(strlen(P->KeyList[i])==0)
					break;
				//strcpy(projectionrecord,Project(b->Re[i],c));
				//printf("%s\n",b->Re[i]);
				Project(b->Re[i],c);
				//printf("!!!!!!!!!2\n");
				//printf("%s\n",projectionrecord);							
				//R->attribute[1]->value
				//TNo,P->BlockID,i
			}
		}		
		P = P->next;		
	}	
}

