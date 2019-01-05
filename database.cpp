#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include <unistd.h>

#include"add.cpp"
#include"delete.cpp"
#include"select.cpp"
#include"table.cpp"
#include"selectoperate.cpp"
#include"index1.cpp"
#include"connect.cpp"
#include"projection.cpp"
//�û�������������һ���� | �ָ����ַ���, �������ݡ�
//PageTable *PTList[MAX_OF_TABLE];//ȫ��ҳ��
//BufferQue *Buf;//ȫ�ֻ�����
BPlusTree BTList[MAX_OF_TABLE];//ȫ��B+��
void AddRecordViaConsole(BPlusTree &BT,PageTable *&PT,BufferQue* &Buf,char inputrec[10240],int TNo)
{
  //  int slength = 0;
  	//printf("!!!!!!!\n");   
    char *ptr;
    char value[MAX_OF_ATTRIBUTE][255];
    int i=0;
	int j=0; 
	   char **val = new char*[255];
    for (int i =0; i < 255; ++i)
      val[i] = new char[255];
    ptr = strtok(inputrec, "|");
	while(ptr){
  //      printf("ptr=%s\n",ptr);
        strcpy(value[i],ptr);
        strcpy(val[i],value[i]);
        i++;
        ptr = strtok(NULL, "|");
    }
  /*  while(i--){
    	    printf("%s",value[i]);
    }*/
 	addRecord(BT,PT,Buf,value[0],val,TNo);
}

//��supplier.tbl�ļ��ж�ȡ���ݲ��������ݿ⡣
void readFileData(BPlusTree &BT,PageTable *&PT,BufferQue* &Buf,char *filename,int TNo)
{
    
	FILE *f;
    char string[10240];
    f = fopen(filename, "r+");
    //f = fopen("supplier.tbl", "r+");
    if(f==NULL)
    {
    	printf("�Ҳ��������ļ���������.\n");
    }
    else{
    	clock_t start,end;
    	start = clock();
    	printf("�ҵ������ļ�.\n");
    	while(fgets(string, 10240, f))
		{
		//    printf("��tbl�ļ���ȡ: %s\n",  string);
		    if(string[strlen(string) - 1] == '\n')
                string[strlen(string) - 1] = '\0' ;
		    AddRecordViaConsole(BT,PT,Buf,string,TNo);
    	}
    	end = clock();
	    printf("���ݱ��ļ����سɹ�����ʱ= %f\n\n", ((double)end-start)/CLK_TCK);
    }
    
}

//������Ϊ����ɾ������
/* 
void DeleteRecordViaConsole(BPlusTree &BT,PageTable *&PT,BufferQue* &Buf,int TNo)
{
	char deletekey[8];
	char enter = '0';
	printf("\n\n������ɾ���ļ�¼������:\n");
	scanf("%s", deletekey);
//	scanf("%c", &enter);
    //for(int i=0;i<1000;i++)
	deleteRecord(BT,PT,Buf,deletekey,TNo);
}
*/
void Operate(int temp);
int SelectOperate(int temp)//ѡ��������������� 
{
	char enter=' ';
	char input[500] = "";
	
	switch(temp){
		case 0:{
			break;
		} 
    /*	case 1:{
    		char condition[500] = "";
    		printf("������Ŀ�����:");
    		scanf("%s",&input);
    		getchar();
    		if(access(input,F_OK) == -1){
    			printf("��%s������\n",input);
			}
			else{
				
				printf("�ҵ���%s\n",input);
				int TNo = -1;
				//�ҵ���Ӧ��� 
				for(int i = 0;i<8;i++){
					if(strcmp(input,TableList[i]) == 0){
						TNo = i;
						break;
					}
				}
				printf("�������ѯ����:");
				gets(condition);
				printf("%s\n",condition);
				printf("����SQL��ѯ���Ϊ��Select * FROM %s WHERE %s\n",input,condition);
				SelectWithOneCondition(BTList[TNo],PTList[TNo],Buf,TNo,condition);
				}
			break;
		}*/
		case 2:{
			char condition1[500] = "";
			char condition2[500] = "";
	  	    printf("������Ŀ�����:");
    		scanf("%s",&input);
    		if(access(input,F_OK) == -1)
				printf("��%s������\n",input);
			else{
				printf("�ҵ���%s\n",input);
				int TNo = -1;
				//�ҵ���Ӧ��� 
				for(int i = 0;i<8;i++){
					if(strcmp(input,TableList[i]) == 0){
						TNo = i;
						break;
					}
				}
				printf("�������ѯ����1:");
				scanf("%s",&condition1);
				printf("�������ѯ����2:");
				scanf("%s",&condition2);
				printf("����SQL��ѯ���Ϊ��Select * FROM %s WHERE %s AND %s\n",input,condition1,condition2);
				getchar();
				SelectWithTwoCondition(BTList[TNo],PTList[TNo],Buf,TNo,condition1,condition2);					
			}
			break;
		} 
		case 3:{
			ConnectOperate(PTList,Buf,0);
			break;
		}
		case 4:{
			ConnectOperate(PTList,Buf,2);
			break;
		}
		case 5:{
			char input1[20],input2[20];
			printf("������Ŀ�����1:");
    		scanf("%s",&input1);
    		printf("������Ŀ�����2:");
    		scanf("%s",&input2);
    		if(access(input1,F_OK) == -1){
    			printf("��%s������\n",input1);
    			break;
			}
			if(access(input2,F_OK) == -1){
    			printf("��%s������\n",input2);
    			break;
			}	
			printf("�ҵ���%s\n",input1);
			printf("�ҵ���%s\n",input2);
			int TNo1 = -1;
			int TNo2 = -1;
			//�ҵ���Ӧ��� 
			int i;
			for(i = 0;i<MAX_OF_TABLE;i++){
				if(strcmp(input1,TableList[i]) == 0){
					TNo1 = i;
					break;
				}
			}
			for(i = 0;i<MAX_OF_TABLE;i++){
				if(strcmp(input2,TableList[i]) == 0){
					TNo2 = i;
					break;
				}
			}
			KaConnect(TNo1,TNo2,Buf,PTList[TNo1],PTList[TNo2]);
			break;
		}
			
		/*case 6:{
			
			
			printf("������Ŀ�����:");
    		scanf("%s",&input);
    		if(access(input,F_OK) == -1)
				printf("��%s������\n",input);
			else{
				printf("�ҵ���%s\n",input);
				int TNo = -1;
				//�ҵ���Ӧ��� 
				for(int i = 0;i<8;i++){
					if(strcmp(input,TableList[i]) == 0){
						TNo = i;
						break;
					}
				}
				char *projectionrecord;
           		projectionrecord=(char *)malloc(255*sizeof(char));
				int bb[Max_Attribute_Num];
				int *c;
				for(int i=0;i<Max_Attribute_Num;i++)
				{ 
				    bb[i]=0;
				}
				c=bb;
				printf("������ͶӰ�����������û��������100\n");
				int k; 
				while(1)
				{ 
				    scanf("%d",&k);
				    if(k==100) break;
				    else
				    {
				    	c=AddAttribute(c,k);
					}
				}
				
				
				clearBuf(Buf);//��ջ�����
				Page *P = PTList[TNo]->next;//ҳ������ҳ 
	
				Record *R= SetRecord(TNo);
				while(P){
					if(strlen(P->KeyList[MAX_OF_RECORD-1]) != 0)
					{
						Block *b=FIFO(PTList[TNo],Buf,P);
					//printf("!!!!!!!!!2\n");
						for(int i = MAX_OF_RECORD-1;i>=0;i--){
							if(strlen(P->KeyList[i])==0)
								break;
							char Rec[500];
							sprintf(Rec,"%.4d%s",TNo,b->Re[i]);
							//printf("%s\n",b->Re[i])	;
						    strcpy(projectionrecord,Project(Rec,c));
							printf("%s\n",projectionrecord);

							
			//R->attribute[1]->value
			//TNo,P->BlockID,i
						}
					}
		
					P = P->next;
			
				}
			}
			
		break;
		}*/
	
		
		
	}
	return temp;
}
/*
void Operate(int temp)//������������ 
{
	char input[500] = ""; 
	char enter = ' ';
	switch(temp)
	{
		case 0:{
			printf("��ջ�����\n");
   			clearBuf(Buf);//��ջ�����
   			printf("��������ճɹ�\n"); 
			int i = 0; 
			printf("�����ҳ��\n");
   			for(i;i < MAX_OF_TABLE;i++){   				
			if(access(TableList[i],F_OK) == 0)
					SavePTIndex(PTList[i],i);

			}
			printf("��ҳ����ɹ�\n");
			i = 0; 
			printf("����B+��\n");
   			for(i;i<MAX_OF_TABLE;i++){
				if(access(TableList[i],F_OK) == 0)
					SaveBPlusTree(BTList[i],i);
			}
			printf("B+������ɹ�\n"); 
			printf("�ɹ��˳����ݿ�\n");
			exit(0);
		}
		case 1:{
			printf("�����뽨�����:\n");
		    scanf("%s",input);
    	    fgets(input,500,stdin);
    	    //printf("%s\n",input);
    	    int TID = CreateTable(input);//������ṹ 
		    PTList[TID] = ReadPTIndex(TID);//ҳ���ʼ��
		    BTList[TID] = Initialize();//B+����ʼ�� 
		    char TablePath[255]="";
		    sprintf(TablePath,"%s//%.4d",TableList[TID],TID);
		    Recinfo[TID] = readtable(TablePath);
			break;
		}
		case 2:{
			printf("������Ŀ�����:");
    	    scanf("%s",&input);
    	    if(access(input,F_OK) == -1)
			    printf("��%s������\n",input);
		    else{
				printf("�ҵ���%s\n",input);
				int TNo = -1;
				char filename[255] = "";
				printf("�����������ļ���:"); 
				scanf("%s",filename);
				//�ҵ���Ӧ��� 
				for(int i = 0;i < MAX_OF_TABLE;i++){
					if(strcmp(input,TableList[i]) == 0){
						TNo = i;
						break;
					}
				}	 
				readFileData(BTList[TNo],PTList[TNo],Buf,filename,TNo);
			}	   	    
			break;
		}
		case 3:{
			printf("������Ŀ�����:");
    		scanf("%s",&input);
    		if(access(input,F_OK) == -1)
				printf("��%s������\n",input);
			else{
				printf("�ҵ���%s\n",input);
				int TNo = -1;
				//�ҵ���Ӧ��� 
				for(int i = 0;i < MAX_OF_TABLE;i++){
					if(strcmp(input,TableList[i]) == 0){
						TNo = i;
						break;
					}
				}
				char enter = '0';
        		printf("\n\n������Ҫ����ļ�¼����'|'�ָ�:\n");
        		char inputrec[10240];
        		scanf("%c", &enter);
        		gets(inputrec);
    	 //   	scanf("%c", &enter);
    			AddRecordViaConsole(BTList[TNo],PTList[TNo],Buf,inputrec,TNo);
    		}			
			break;
		}
		case 4:{
			printf("������Ŀ�����:");
    		scanf("%s",&input);
    		if(access(input,F_OK) == -1)
				printf("��%s������\n",input);
			else{
				printf("�ҵ���%s\n",input);
				int TNo = -1;
				//�ҵ���Ӧ��� 
				for(int i = 0;i<8;i++){
					if(strcmp(input,TableList[i]) == 0){
						TNo = i;
						break;
					}
				}
				DeleteRecordViaConsole(BTList[TNo],PTList[TNo],Buf,TNo);
    			//scanf("%c", &temp);
    		}
			break;
		}
		case 5:{
			printf("������Ŀ�����:");
    		scanf("%s",&input);
    		if(access(input,F_OK) == -1)
				printf("��%s������\n",input);
			else{
				printf("�ҵ���%s\n",input);
				int TNo = -1;
				//�ҵ���Ӧ��� 
				for(int i = 0;i < MAX_OF_TABLE;i++){
					if(strcmp(input,TableList[i]) == 0){
						TNo = i;
						break;
					}
				}
	     	    char enter = '0';
	     	    char updatekey[8];
	      	    printf("\n\n������Ҫ�޸ĵļ�¼������:\n");
        	    scanf("%s", updatekey);
	      	    scanf("%c", &enter);
           	    //for(int i=0;i<1000;i++)
	        	deleteRecord(BTList[TNo],PTList[TNo],Buf,updatekey,TNo);
        		printf("\n\n�������޸ĺ�ļ�¼����'|'�ָ�:\n");
        		char updaterec[10240];
        		scanf("%s", updaterec);
        		scanf("%c", &enter);
        		AddRecordViaConsole(BTList[TNo],PTList[TNo],Buf,updaterec,TNo);
        		printf("\n�޸ļ�¼�ɹ���\n");
    		}
			break;
		}
		case 6:{
			int t=1;
			while(t){
				printf("\n��ѡ�����Ĳ���");
        		printf("\n1.�ڵ����ϵĵ�����ѡ��\n2.�ڵ����ϵ���������ѡ��.\n3.������������\n4.�������������.\n5.��ʽ��.\n6.ͶӰ\n0.������һ��.\n");
    			scanf("%d",&t);
				t=SelectOperate(t);
			}
			break;
		}
		case 7:{

			printf("������Ŀ�����:");
    		scanf("%s",&input);
    		if(access(input,F_OK) == -1)
				printf("��%s������\n",input);
			else{
				printf("�ҵ���%s\n",input);
				int TNo = -1;
				//�ҵ���Ӧ��� 
				for(int i = 0;i <MAX_OF_TABLE;i++){
					if(strcmp(input,TableList[i]) == 0){
						TNo = i;
						break;
					}
				}
				int ANo = 0;
				CharBPlusTree CBT = InitializeCharTree();
				createCharTree(CBT,PTList[TNo],Buf,TNo,ANo);
				printf("char���������ɹ�\n");
				TravelCharTree(CBT);
			}
			break;
		}
	}
} 
*/
/* 
int main(){
	//FILE* bufLog=fopen("log/buflog","a+");//��������־ 
    //FILE* pageLog=fopen("log/pagelog","a+");//ҳ��־ 
    //FILE* blockLog=fopen("log/blocklog","a+");//���ݿ���־ 
    
	
	
	int i;
	printf("��������ʼ��\n"); 
	Buf = InitBuffer();//��������ʼ��
	printf("��������ʼ���ɹ�\n");
	//ҳ��B+����ʼ�� 
	printf("��ҳ���ʼ��\n");
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
	printf("��ҳ���ʼ���ɹ�\n");
	
	printf("B+����ʼ��\n");
	for(i = 0;i < MAX_OF_TABLE;i++){
		int TNo = i;
		if(access(TableList[TNo],F_OK) == -1)
		{
			BTList[TNo] = NULL;
			//printf("%d\n",PTList[i]==NULL);
		}	
		else{			
			BTList[TNo] = ReadBPlusTree(TNo);
		}
	}
	printf("B+����ʼ���ɹ�\n");
	
	initrecinfo();
	//printf("!!!\n");
	int temp;
	//ReadTable
	while(true){
		printf("\n������ѡ��");
   		printf("\n1.�����ݿ��д�����\n2.����м��������ļ�.\n3.��������һ����¼\n4.�ӱ���ɾ��һ����¼.\n5.�ڱ����޸ļ�¼\n6.�ڱ��в�ѯ��¼.\n7.�鿴�����ṹ\n0.�˳�.\n");
    	scanf("%d",&temp);
		Operate(temp);
	}	
	
	
    return 0;
}
*/


