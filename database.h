//database.h
#pragma once 
#include <stdbool.h>
#define PAGENUM 2100//页表大小 
#define BufLength 20//缓冲区大小 
#define SIZE_OF_BLOCK 81920//819200 //每个块的大小，8kb，最后一块可能不到8kb，每块会空余一些空间
#define MAX_OF_RECORD 200 //每块记录最长为200条
#define MAX_OF_ATTRIBUTE 20//每张表最多20条属性
#define MAX_OF_TABLE 8//数据库最多8张表 
#define M (100)
#define LIMIT_M_2 (M % 2 ? (M + 1)/2 : M/2)
#define Max_Attribute_Num 20
const char *TableList[] = {"NATION","REGION","PART","SUPPLIER","PARTSUPP","CUSTOMER","ORDERS","LINEITEM"};
const char *TypeList[] = {"unset","char","varchar","int","long","float","double","date"}; 
int DELETE_SIGN;

