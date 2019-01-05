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
//用户从命令行输入一串由 | 分隔的字符串, 存入数据。
//PageTable *PTList[MAX_OF_TABLE];//全局页表
//BufferQue *Buf;//全局缓冲区
BPlusTree BTList[MAX_OF_TABLE];//全局B+树
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

//从supplier.tbl文件中读取数据并放入数据库。
void readFileData(BPlusTree &BT,PageTable *&PT,BufferQue* &Buf,char *filename,int TNo)
{
    
	FILE *f;
    char string[10240];
    f = fopen(filename, "r+");
    //f = fopen("supplier.tbl", "r+");
    if(f==NULL)
    {
    	printf("找不到数据文件，请重试.\n");
    }
    else{
    	clock_t start,end;
    	start = clock();
    	printf("找到数据文件.\n");
    	while(fgets(string, 10240, f))
		{
		//    printf("从tbl文件读取: %s\n",  string);
		    if(string[strlen(string) - 1] == '\n')
                string[strlen(string) - 1] = '\0' ;
		    AddRecordViaConsole(BT,PT,Buf,string,TNo);
    	}
    	end = clock();
	    printf("数据表文件加载成功。耗时= %f\n\n", ((double)end-start)/CLK_TCK);
    }
    
}

//以主键为索引删除数据
/* 
void DeleteRecordViaConsole(BPlusTree &BT,PageTable *&PT,BufferQue* &Buf,int TNo)
{
	char deletekey[8];
	char enter = '0';
	printf("\n\n请输入删除的记录的主键:\n");
	scanf("%s", deletekey);
//	scanf("%c", &enter);
    //for(int i=0;i<1000;i++)
	deleteRecord(BT,PT,Buf,deletekey,TNo);
}
*/
void Operate(int temp);
int SelectOperate(int temp)//选择操作的引导函数 
{
	char enter=' ';
	char input[500] = "";
	
	switch(temp){
		case 0:{
			break;
		} 
    /*	case 1:{
    		char condition[500] = "";
    		printf("请输入目标表名:");
    		scanf("%s",&input);
    		getchar();
    		if(access(input,F_OK) == -1){
    			printf("表%s不存在\n",input);
			}
			else{
				
				printf("找到表%s\n",input);
				int TNo = -1;
				//找到对应表号 
				for(int i = 0;i<8;i++){
					if(strcmp(input,TableList[i]) == 0){
						TNo = i;
						break;
					}
				}
				printf("请输入查询条件:");
				gets(condition);
				printf("%s\n",condition);
				printf("您的SQL查询语句为：Select * FROM %s WHERE %s\n",input,condition);
				SelectWithOneCondition(BTList[TNo],PTList[TNo],Buf,TNo,condition);
				}
			break;
		}*/
		case 2:{
			char condition1[500] = "";
			char condition2[500] = "";
	  	    printf("请输入目标表名:");
    		scanf("%s",&input);
    		if(access(input,F_OK) == -1)
				printf("表%s不存在\n",input);
			else{
				printf("找到表%s\n",input);
				int TNo = -1;
				//找到对应表号 
				for(int i = 0;i<8;i++){
					if(strcmp(input,TableList[i]) == 0){
						TNo = i;
						break;
					}
				}
				printf("请输入查询条件1:");
				scanf("%s",&condition1);
				printf("请输入查询条件2:");
				scanf("%s",&condition2);
				printf("您的SQL查询语句为：Select * FROM %s WHERE %s AND %s\n",input,condition1,condition2);
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
			printf("请输入目标表名1:");
    		scanf("%s",&input1);
    		printf("请输入目标表名2:");
    		scanf("%s",&input2);
    		if(access(input1,F_OK) == -1){
    			printf("表%s不存在\n",input1);
    			break;
			}
			if(access(input2,F_OK) == -1){
    			printf("表%s不存在\n",input2);
    			break;
			}	
			printf("找到表%s\n",input1);
			printf("找到表%s\n",input2);
			int TNo1 = -1;
			int TNo2 = -1;
			//找到对应表号 
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
			
			
			printf("请输入目标表名:");
    		scanf("%s",&input);
    		if(access(input,F_OK) == -1)
				printf("表%s不存在\n",input);
			else{
				printf("找到表%s\n",input);
				int TNo = -1;
				//找到对应表号 
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
				printf("请输入投影的列数，如果没有请输入100\n");
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
				
				
				clearBuf(Buf);//清空缓冲区
				Page *P = PTList[TNo]->next;//页表中首页 
	
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
void Operate(int temp)//操作引导函数 
{
	char input[500] = ""; 
	char enter = ' ';
	switch(temp)
	{
		case 0:{
			printf("清空缓冲区\n");
   			clearBuf(Buf);//清空缓冲区
   			printf("缓冲区清空成功\n"); 
			int i = 0; 
			printf("保存段页表\n");
   			for(i;i < MAX_OF_TABLE;i++){   				
			if(access(TableList[i],F_OK) == 0)
					SavePTIndex(PTList[i],i);

			}
			printf("段页表保存成功\n");
			i = 0; 
			printf("保存B+树\n");
   			for(i;i<MAX_OF_TABLE;i++){
				if(access(TableList[i],F_OK) == 0)
					SaveBPlusTree(BTList[i],i);
			}
			printf("B+树保存成功\n"); 
			printf("成功退出数据库\n");
			exit(0);
		}
		case 1:{
			printf("请输入建表语句:\n");
		    scanf("%s",input);
    	    fgets(input,500,stdin);
    	    //printf("%s\n",input);
    	    int TID = CreateTable(input);//创建表结构 
		    PTList[TID] = ReadPTIndex(TID);//页表初始化
		    BTList[TID] = Initialize();//B+树初始化 
		    char TablePath[255]="";
		    sprintf(TablePath,"%s//%.4d",TableList[TID],TID);
		    Recinfo[TID] = readtable(TablePath);
			break;
		}
		case 2:{
			printf("请输入目标表名:");
    	    scanf("%s",&input);
    	    if(access(input,F_OK) == -1)
			    printf("表%s不存在\n",input);
		    else{
				printf("找到表%s\n",input);
				int TNo = -1;
				char filename[255] = "";
				printf("请输入数据文件名:"); 
				scanf("%s",filename);
				//找到对应表号 
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
			printf("请输入目标表名:");
    		scanf("%s",&input);
    		if(access(input,F_OK) == -1)
				printf("表%s不存在\n",input);
			else{
				printf("找到表%s\n",input);
				int TNo = -1;
				//找到对应表号 
				for(int i = 0;i < MAX_OF_TABLE;i++){
					if(strcmp(input,TableList[i]) == 0){
						TNo = i;
						break;
					}
				}
				char enter = '0';
        		printf("\n\n请输入要插入的记录。用'|'分隔:\n");
        		char inputrec[10240];
        		scanf("%c", &enter);
        		gets(inputrec);
    	 //   	scanf("%c", &enter);
    			AddRecordViaConsole(BTList[TNo],PTList[TNo],Buf,inputrec,TNo);
    		}			
			break;
		}
		case 4:{
			printf("请输入目标表名:");
    		scanf("%s",&input);
    		if(access(input,F_OK) == -1)
				printf("表%s不存在\n",input);
			else{
				printf("找到表%s\n",input);
				int TNo = -1;
				//找到对应表号 
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
			printf("请输入目标表名:");
    		scanf("%s",&input);
    		if(access(input,F_OK) == -1)
				printf("表%s不存在\n",input);
			else{
				printf("找到表%s\n",input);
				int TNo = -1;
				//找到对应表号 
				for(int i = 0;i < MAX_OF_TABLE;i++){
					if(strcmp(input,TableList[i]) == 0){
						TNo = i;
						break;
					}
				}
	     	    char enter = '0';
	     	    char updatekey[8];
	      	    printf("\n\n请输入要修改的记录的主键:\n");
        	    scanf("%s", updatekey);
	      	    scanf("%c", &enter);
           	    //for(int i=0;i<1000;i++)
	        	deleteRecord(BTList[TNo],PTList[TNo],Buf,updatekey,TNo);
        		printf("\n\n请输入修改后的记录。用'|'分隔:\n");
        		char updaterec[10240];
        		scanf("%s", updaterec);
        		scanf("%c", &enter);
        		AddRecordViaConsole(BTList[TNo],PTList[TNo],Buf,updaterec,TNo);
        		printf("\n修改记录成功！\n");
    		}
			break;
		}
		case 6:{
			int t=1;
			while(t){
				printf("\n请选择您的操作");
        		printf("\n1.在单表上的单条件选择\n2.在单表上的两个条件选择.\n3.两表单条件连接\n4.两表多条件连接.\n5.卡式积.\n6.投影\n0.返回上一级.\n");
    			scanf("%d",&t);
				t=SelectOperate(t);
			}
			break;
		}
		case 7:{

			printf("请输入目标表名:");
    		scanf("%s",&input);
    		if(access(input,F_OK) == -1)
				printf("表%s不存在\n",input);
			else{
				printf("找到表%s\n",input);
				int TNo = -1;
				//找到对应表号 
				for(int i = 0;i <MAX_OF_TABLE;i++){
					if(strcmp(input,TableList[i]) == 0){
						TNo = i;
						break;
					}
				}
				int ANo = 0;
				CharBPlusTree CBT = InitializeCharTree();
				createCharTree(CBT,PTList[TNo],Buf,TNo,ANo);
				printf("char索引创建成功\n");
				TravelCharTree(CBT);
			}
			break;
		}
	}
} 
*/
/* 
int main(){
	//FILE* bufLog=fopen("log/buflog","a+");//缓冲区日志 
    //FILE* pageLog=fopen("log/pagelog","a+");//页日志 
    //FILE* blockLog=fopen("log/blocklog","a+");//数据块日志 
    
	
	
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
	
	printf("B+树初始化\n");
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
	printf("B+树初始化成功\n");
	
	initrecinfo();
	//printf("!!!\n");
	int temp;
	//ReadTable
	while(true){
		printf("\n请做出选择");
   		printf("\n1.在数据库中创建表\n2.向表中加载数据文件.\n3.向表中添加一个记录\n4.从表中删除一个记录.\n5.在表中修改记录\n6.在表中查询记录.\n7.查看索引结构\n0.退出.\n");
    	scanf("%d",&temp);
		Operate(temp);
	}	
	
	
    return 0;
}
*/


