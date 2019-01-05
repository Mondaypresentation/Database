#pragma once 
#include"record.cpp"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#define MAX_OF_TABLE 8//数据库最多10张表 
//const int TABLE_NUM = 1;
Record *Recinfo[MAX_OF_TABLE];
char *change(int a)//将数字转化为四位的字符串 
{
	char b[4];
	char *tar=(char*)malloc(sizeof(char));
	int temp;
	temp=a/1000;
	b[0]=temp+48;
	a=a-temp*1000 ;
	temp=a/100;
	b[1]=temp+48;
	a=a-temp*100;
	temp=a/10;
	b[2]=temp+48;
	a=a-temp*10;
	temp=a;
	b[3]=temp+48;
	strncpy(tar,b,4);
	return tar;
 } 
Record *readtable(char *filename)//读取表中的记录属性信息 
{
	char StrLine[1024];
	char temp1[4],temp2[4],temp3[4],temp4[20],nums[2];
	int count=0,i,j;
	int k=0;	
	FILE *fp=fopen(filename,"r");
	if(fp == NULL)
    {
     	return NULL;
    }
	fgets(StrLine,1024,fp);
	//printf("%s\n",StrLine);		
	strncpy(nums,StrLine,1);
	count=atoi(nums);
	//printf("%d\n",count);	
	Record *set=SetRecord(count);
    set->Head->attrubuteNum=count;
//    printf("111\n");
    for(i=0;i<count;i++)
    { 
    	fgets(StrLine,1024,fp);  //读取一行
    	strncpy(temp1,StrLine,1);
    	set->attribute[i]->type=atoi(temp1);
        strncpy(temp2,StrLine+2,1);
        if(atoi(temp2)==1)
    		set->attribute[i]->IFVar=1;
    	else
    		set->attribute[i]->IFVar=0;
    	strncpy(temp3,StrLine+4,4);
    	set->attribute[i]->AttributeLength=atoi(temp3);
    	strcpy(temp4,StrLine+9);	
    	strncpy(set->attribute[i]->name,temp4,strlen(temp4)-1);
    }
    fclose(fp);
    return set;
 } 
void initrecinfo()//初始化所有表的信息 
{
	char *temp=(char*)malloc(sizeof(char));
	for(int i=0;i<MAX_OF_TABLE;i++)
	{
		//printf("!%d\n",i);
		strcpy(temp,change(i));
		char TablePath[255];
		sprintf(TablePath,"%.4d",i);
		//printf("!!!!!!\n");
		Recinfo[i]=readtable(TablePath);
		//printf("!!!!!!\n");
		if(Recinfo[i] !=NULL)
			strcpy(Recinfo[i]->Head->tablenum,temp);
		
	}
	//printf("!!!!!!\n");
 } 
Record *load(char *table,char **input)//输入Attribute中的value值的二维字符串数组和表的文件名，返回Record
{
//	printf("%d",atoi(table));
	for(int i=0;i<Recinfo[atoi(table)]->Head->attrubuteNum;i++)
	{	
		//printf("%s!!!\n",input[i]); 
		strcpy(Recinfo[atoi(table)]->attribute[i]->value,input[i]);
	}
	return Recinfo[atoi(table)];
} 
char *input(Record *rec)//将记录转化为字符串 
{	
	char*b;//记录头空四位 
	char *p=(char*)malloc(510*sizeof(char));
	//(char*)memset(p,0x00,510*sizeof(char));
	//char p[510]; 
	char *stop=(char*)malloc(sizeof(char));
	strcpy(stop,";");
	char name1[20];	
	int sum=0,total=0,temp,a;
	strcpy(p,rec->Head->tablenum);
	for(int i=0;i<rec->Head->attrubuteNum;i++)//计算记录长度和偏移量，sum计算 偏移量，total计算记录长度 
	{
		total=total+4+strlen(rec->attribute[i]->value)+1;
	}
	b=change(total);
	rec->Head->RecordLength=total;
	//printf("%d %s\n",total,b);
	//strcat(p,b);
	strcpy(p+4,b);
	//printf("%s\n",p);
	for(int i=0;i<rec->Head->attrubuteNum;i++)//记录属性 
	{
			strcat(p,rec->attribute[i]->value);
			strcat(p,stop);			
	}
	//printf("%s\n",p);
	free(stop);
	return p;
}
Record *output(char *line)//将字符串形式的数组转化为Record 
{
	char filename[10];
	strncpy(filename,line,4);
//	printf("%d\n",Recinfo[atoi(filename)]->Head->attrubuteNum); 
	Record *outrec =SetRecord(Recinfo[atoi(filename)]->Head->attrubuteNum);
//	printf("1\n");
	int i,cal=8,test;
//	char *temp=(char*)malloc(255*sizeof(char));
	for(i=0;i<Recinfo[atoi(filename)]->Head->attrubuteNum;i++)
	{
		strcpy(outrec->attribute[i]->name,Recinfo[atoi(filename)]->attribute[i]->name);
		outrec->attribute[i]->AttributeLength= Recinfo[atoi(filename)]->attribute[i]->AttributeLength;
		outrec->attribute[i]->type=Recinfo[atoi(filename)]->attribute[i]->type;
		outrec->attribute[i]->IFVar=Recinfo[atoi(filename)]->attribute[i]->IFVar;
//		printf("%s %d\n",Recinfo[atoi(filename)]->attribute[i]->name,Recinfo[atoi(filename)]->attribute[i]->IFVar);
//		printf("%s %d\n",outrec->attribute[i]->name,outrec->attribute[i]->IFVar);
	}
//	printf("2\n");
	outrec->Head->attrubuteNum=Recinfo[atoi(filename)]->Head->attrubuteNum;
	strcpy(outrec->Head->tablenum,Recinfo[atoi(filename)]->Head->tablenum);
//	printf("%d",outrec->Head->attrubuteNum);
//	printf("3\n");
	int j=8;
	for(i=0;i<outrec->Head->attrubuteNum;i++)
	 {
	 //	printf("%d %s %d\n",i,outrec->attribute[i]->name,outrec->attribute[i]->IFVar);
	 //	printf("%d %s %d\n",i,outrec->attribute[1]->name,outrec->attribute[1]->IFVar);
	 //	printf("%d %s %d\n",i,outrec->attribute[3]->name,outrec->attribute[3]->IFVar);	
		//	printf("6\n");
			int o=0;
//			printf("%d\n",j);
			while(line[j]!=59)
			{	
//				printf("%c",line[j]);
				outrec->attribute[i]->value[o]=line[j];
				o++;
				j++;
			}
//			printf("\n");
//			printf("%d\n",strlen(outrec->attribute[i]->value));
			j++;
			//j=j+strlen(outrec->attribute[i]->value);
			//printf("%d %s\n",i,outrec->attribute[i]->value);
	 }
	 
	 return outrec;
}
Record *recjoin(char *table1,char *table2)
{
	int sum=0,t=1;
	sum=Recinfo[atoi(table1)]->Head->attrubuteNum+Recinfo[atoi(table2)]->Head->attrubuteNum+1;

	Record *outrec =SetRecord(sum);
	outrec->Head->attrubuteNum=sum;
	strcpy(outrec->attribute[0]->name,"NO");
	strcpy(outrec->attribute[0]->value,"0");
	outrec->attribute[0]->AttributeLength= 8;
	outrec->attribute[0]->type=3;
	outrec->attribute[0]->IFVar=1;	
	for(int i=0;i<Recinfo[atoi(table1)]->Head->attrubuteNum;i++)
	{
		strcpy(outrec->attribute[t]->name,Recinfo[atoi(table1)]->attribute[i]->name);
		outrec->attribute[t]->AttributeLength= Recinfo[atoi(table1)]->attribute[i]->AttributeLength;
		outrec->attribute[t]->type=Recinfo[atoi(table1)]->attribute[i]->type;
		outrec->attribute[t]->IFVar=Recinfo[atoi(table1)]->attribute[i]->IFVar;
		t++;
	}
	for(int i=0;i<Recinfo[atoi(table2)]->Head->attrubuteNum;i++)
	{
		strcpy(outrec->attribute[t]->name,Recinfo[atoi(table2)]->attribute[i]->name);
		outrec->attribute[t]->AttributeLength= Recinfo[atoi(table2)]->attribute[i]->AttributeLength;
		outrec->attribute[t]->type=Recinfo[atoi(table2)]->attribute[i]->type;
		outrec->attribute[t]->IFVar=Recinfo[atoi(table2)]->attribute[i]->IFVar;
		t++;
	}
	return outrec;
}
Record *Recjoin(Record *rec1,Record *rec2,Record *rec12,int &sum)
{
	int t=1;
	sum++;
	char s[8];
	sprintf(s,"%d",sum);
	strcpy(rec12->attribute[0]->value,s);
	for(int i=0;i<rec1->Head->attrubuteNum;i++)
	{
		strcpy(rec12->attribute[t]->value,rec1->attribute[i]->value);
		t++;
	}
		for(int i=0;i<rec2->Head->attrubuteNum;i++)
	{
		strcpy(rec12->attribute[t]->value,rec2->attribute[i]->value);
		t++;
	}
	return rec12;
	//rec12->attribute[0]->value
}
/*
int main()
{
	initrecinfo();
//	printf("%s\n",Recinfo[0]->Head->tablenum);
	char *c[]={"1","Supplier#000000001"," N kD4on9OM Ipw3,gf0JBoQDd7tgrzrddZ","17","27-918-335-1736","5755.94","each slyly above the careful"};
	char  *d[]={"1","goldenrod lavender spring chocolate lace","Manufacturer#1","Brand#13","PROMO BURNISHED COPPER","7","JUMBO PKG","901.00","ly. slyly ironi"};
	char *q[]={"0","CANADA","0"," haggle. carefully final deposits detect slyly agai"} ;
	char **b=c;
//	load("0003",b);
//	printf("!!!!!!!\n");
//	Record *a=InitRecord();
//	printf("!!!!!!!\n");
//	a=recjoin("0003","0002");
//	Record *a1=InitRecord();
//	Record *a2=InitRecord();
	
//	b=d;
//	a2=load("0002",b);
	Record *tplink =InitRecord();
//	tplink =Recjoin(a1,a2,a);
	tplink=load("0003",b);
//	for(int i=0;i<tplink->Head->attrubuteNum;i++)
//	{
//		printf("%d %d %d %s %s\n",tplink->attribute[i]->type,tplink->attribute[i]->IFVar,tplink->attribute[i]->AttributeLength,tplink->attribute[i]->name,tplink->attribute[i]->value);
//	}	
	char *sqwe=(char*)malloc(500*sizeof(char));
//	strcpy(tplink->Head->tablenum,"0001");
	sqwe=input(tplink);
	printf("%s\n",sqwe);
	Record *a1=InitRecord();
	a1=output(sqwe);
	for(int i=0;i<a1->Head->attrubuteNum;i++)
	{
		printf("%d %d %d %s %s\n",a1->attribute[i]->type,a1->attribute[i]->IFVar,a1->attribute[i]->AttributeLength,a1->attribute[i]->name,a1->attribute[i]->value);
	}
}*/
