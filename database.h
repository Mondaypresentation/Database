//database.h
#pragma once 
#include <stdbool.h>
#define PAGENUM 2100//ҳ���С 
#define BufLength 50//��������С 
#define SIZE_OF_BLOCK 81920//819200 //ÿ����Ĵ�С��8kb�����һ����ܲ���8kb��ÿ������һЩ�ռ�
#define MAX_OF_RECORD 200 //ÿ���¼�Ϊ200��
#define MAX_OF_ATTRIBUTE 20//ÿ�ű����20������
#define MAX_OF_TABLE 8//���ݿ����8�ű� 
#define M (100)
#define LIMIT_M_2 (M % 2 ? (M + 1)/2 : M/2)
#define Max_Attribute_Num 20
const char *TableList[] = {"NATION","REGION","PART","SUPPLIER","PARTSUPP","CUSTOMER","ORDERS","LINEITEM"};
const char *TypeList[] = {"unset","char","varchar","int","long","float","double","date"}; 
int DELETE_SIGN;

