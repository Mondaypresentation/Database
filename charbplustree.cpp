#pragma once 
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include<string.h>
#include"page.cpp"
#include "database.h"
#include"select.cpp"
#include"projection.cpp"
#include"delete.cpp"
typedef struct CharBPlusNode *CharBPlusTree,*CharPosition;

/*B+���Ľڵ�ṹ*/ 
struct CharBPlusNode{
    int KeyNum;//��ֵ���� 
    char *Key[M + 1];//��ֵ���� 
    int tableno[M + 1];
	int blockno[M + 1];
	int offset[M + 1]; 
    CharBPlusTree Children[M + 1];//�ӽڵ� 
    CharBPlusTree Next;//�ֵܽڵ� 
};
 
static int Unavailable1 = INT_MIN;

/* ���ɽڵ㲢��ʼ�� */
static CharBPlusTree MallocNewCharNode(){
    CharBPlusTree NewNode;
    int i;
    NewNode = (CharBPlusNode*)malloc(sizeof(CharBPlusNode));
    if (NewNode == NULL)
        exit(EXIT_FAILURE);
    i = 0;
    while (i < M + 1){
    	NewNode->Key[i]=(char*)malloc(255*sizeof(char));//���ռ�char[255] 
        strcpy(NewNode->Key[i],"-111111111") ;
        NewNode->tableno[i]=-1;
        NewNode->blockno[i]=-1;
        NewNode->offset[i]=-1;
        NewNode->Children[i] = NULL;
        i++;
    }
    NewNode->Next = NULL;
    NewNode->KeyNum = 0;  
    return NewNode;
}
 
/* ��ʼ�� */
extern CharBPlusTree InitializeCharTree(){
    CharBPlusTree T;    
    if (M < (3)){
        printf("M��С����3��");
        exit(EXIT_FAILURE);
        
    }
    /* ����� */
    T = MallocNewCharNode();
    return T;
}
/*�ҵ�����ߵ�Ҷ�ӽڵ�*/ 
static CharPosition FindMostLeftChar(CharPosition P){
    CharPosition Tmp;
    Tmp = P;
    while (Tmp != NULL && Tmp->Children[0] != NULL) {
        Tmp = Tmp->Children[0];
    }
    return Tmp;
}
/*�ҵ����ұߵ�Ҷ�ӽڵ�*/ 
static CharPosition FindMostRightChar(CharPosition P){
    CharPosition Tmp;
    Tmp = P;
    while (Tmp != NULL && Tmp->Children[Tmp->KeyNum - 1] != NULL) {
        Tmp = Tmp->Children[Tmp->KeyNum - 1];
    }
    return Tmp;
}
/*Ѱ��parent��children[i]��һ���ֵܽڵ㣬��洢�Ĺؼ���δ�������򷵻�NULL */
static CharPosition FindSiblingChar(CharPosition Parent,int i){
    CharPosition Sibling;
    int Limit;
    Limit = M;
    Sibling = NULL;
    if (i == 0){
        if (Parent->Children[1]->KeyNum < Limit)//���i�ǵ�һ���������ұ߽ڵ� 
            Sibling = Parent->Children[1];
    }
    else if (Parent->Children[i - 1]->KeyNum < Limit) //����i����� 
        Sibling = Parent->Children[i - 1];
    else if (i + 1 < Parent->KeyNum && Parent->Children[i + 1]->KeyNum < Limit)//��i���ұ� 
	{
        Sibling = Parent->Children[i + 1];
    }
    return Sibling;
}
/* �����ֵܽڵ㣬��ؼ���������M/2 ;û�з���NULL*/
static CharPosition FindSiblingCharKeyNum_M_2(CharPosition Parent,int i,int *j){
    int Limit;
    CharPosition Sibling;
    Sibling = NULL;    
    Limit = LIMIT_M_2;    
    if (i == 0){
        if (Parent->Children[1]->KeyNum > Limit){
            Sibling = Parent->Children[1];
            *j = 1;
        }
    }
    else{
        if (Parent->Children[i - 1]->KeyNum > Limit){
            Sibling = Parent->Children[i - 1];
            *j = i - 1;
        }
        else if (i + 1 < Parent->KeyNum && Parent->Children[i + 1]->KeyNum > Limit){
            Sibling = Parent->Children[i + 1];
            *j = i + 1;
        }
        
    }
    return Sibling;
}
/* ��Ҫ��X����Key��ʱ��i��X��Parent��λ�ã�j��KeyҪ�����λ��
   ��Ҫ��Parent����X�ڵ��ʱ��i��Ҫ�����λ�ã�Key��j��ֵû����
*/
static CharPosition InsertElementChar(int isKey, CharPosition Parent,CharPosition X,char *Key,int i,int j,int tno,int bno,int ofset){
    
    int k;
    if (isKey){
        /* ����key */
        k = X->KeyNum - 1;
        while (k >= j){
            strcpy(X->Key[k + 1],X->Key[k]);
            X->tableno[k + 1] = X->tableno[k];
			X->blockno[k + 1] = X->blockno[k];
			X->offset[k + 1] = X->offset[k];
			k--;
        }
        strcpy(X->Key[j],Key); 
        X->tableno[j]=tno;
        X->blockno[j]=bno;
        X->offset[j]=ofset;
        if (Parent != NULL)
            {
			strcpy(Parent->Key[i],X->Key[0]);
            Parent->tableno[i]=X->tableno[0];
		    Parent->blockno[i]=X->blockno[0];
		    Parent->offset[i]=X->offset[0];
        	
        	}
		X->KeyNum++;
    }else{
        /* ����ڵ� */
        /* ����Ҷ�ڵ�������� */
        if (X->Children[0] == NULL)//x��Ҷ�ӽڵ� 
		{
            if (i > 0)
                Parent->Children[i - 1]->Next = X;
            X->Next = Parent->Children[i];
        }
        k = Parent->KeyNum - 1;
        //�ƶ����ڵ���ֵ������x 
        while (k >= i){
            Parent->Children[k + 1] = Parent->Children[k];
            strcpy(Parent->Key[k + 1],Parent->Key[k]);
            Parent->tableno[k + 1] = Parent->tableno[k];
            Parent->blockno[k + 1] = Parent->blockno[k];
            Parent->offset[k + 1] = Parent->offset[k];
            k--;
        }
        strcpy(Parent->Key[i],X->Key[0]);
        Parent->tableno[i] = X->tableno[0];
        Parent->blockno[i] = X->blockno[0];
        Parent->offset[i] = X->offset[0];
        Parent->Children[i] = X;
        Parent->KeyNum++;   
    }
    //printf("11111!!!!!!\n");
    return X;
}
 
/*��Ҫ��X�Ƴ�Key��ʱ��i��X��Parent��λ�ã�j��KeyҪ�Ƴ���λ��
   ��Ҫ��Parent�Ƴ�X�ڵ��ʱ��i��Ҫ�Ƴ���λ�ã�Key��j��ֵû����
*/ 
static CharPosition RemoveElementChar(int isKey, CharPosition Parent,CharPosition X,int i,int j){
    
    int k,Limit;
    
    if (isKey){
        Limit = X->KeyNum;
        /* ɾ��key */
        k = j + 1;
        while (k < Limit){
        	X->tableno[k - 1] = X->tableno[k];
			X->blockno[k - 1] = X->blockno[k];
			X->offset[k - 1] = X->offset[k];
            strcpy(X->Key[k - 1],X->Key[k]);
			k++;
        }
        
        strcpy(X->Key[X->KeyNum - 1],"-111111111");
        X->tableno[X->KeyNum - 1]=-1;
        X->blockno[X->KeyNum - 1]=-1;
        X->offset[X->KeyNum - 1]=-1;
        strcpy(Parent->Key[i],X->Key[0]);
        Parent->tableno[i] = X->tableno[0];
        Parent->offset[i] = X->offset[0];
        Parent->blockno[i] = X->blockno[0];
        X->KeyNum--;
    }else{
        /* ɾ���ڵ� */
        
        /* �޸���Ҷ�ڵ������ */
        if (X->Children[0] == NULL && i > 0)// �ж�x�ǲ���Ҷ�ӽڵ� 
		{
            Parent->Children[i - 1]->Next = Parent->Children[i + 1];
        }
        Limit = Parent->KeyNum;
        k = i + 1;
        while (k < Limit){
            Parent->Children[k - 1] = Parent->Children[k];
            strcpy(Parent->Key[k - 1],Parent->Key[k]);
            Parent->tableno[k - 1] = Parent->tableno[k];
            Parent->blockno[k - 1] = Parent->blockno[k];
            Parent->offset[k - 1] = Parent->offset[k];
            k++;
        }
        Parent->Children[Parent->KeyNum - 1] = NULL;
        strcpy(Parent->Key[Parent->KeyNum - 1],"-111111111");
        Parent->tableno[Parent->KeyNum - 1] = -1;
        Parent->blockno[Parent->KeyNum - 1] = -1;
        Parent->offset[Parent->KeyNum - 1] = -1;
        Parent->KeyNum--;
    }
    return X;
}
 
/* Src��Dst���������ڵĽڵ㣬i��Src��Parent�е�λ�ã�
 ��Src��Ԫ���ƶ���Dst�� ,n���ƶ�Ԫ�صĸ���*/
static CharPosition MoveElementChar(CharPosition Src,CharPosition Dst,CharPosition Parent,int i,int n){
    char *TmpKey;
    TmpKey=(char*)malloc(255*sizeof(char));
    CharPosition Child;
    int j,SrcInFront;
    int tno,bno,ofset;
    
    SrcInFront = 0;
    
    if (strcmp(Src->Key[0],Dst->Key[0])<0)
        SrcInFront = 1;
    
    j = 0;
    /* �ڵ�Src��Dstǰ�� */
    if (SrcInFront){
        if (Src->Children[0] != NULL)//Src����Ҷ�ӽڵ� 
		{
            while (j < n) {
                Child = Src->Children[Src->KeyNum - 1];//child��src���һ���ӽڵ� 
                RemoveElementChar(0, Src, Child, Src->KeyNum - 1, Unavailable1);//�Ƴ�src��child 
                InsertElementChar(0, Dst, Child, "-111111111", 0, Unavailable1,-1,-1,-1);//��child����dst 
                j++;
            }
        }else{
            while (j < n) {//Ҷ�ӽڵ�ı丳ֵ��� 
                strcpy(TmpKey,Src->Key[Src->KeyNum -1]);
                tno=Src->tableno[Src->KeyNum -1];
                bno=Src->blockno[Src->KeyNum -1];
                ofset=Src->offset[Src->KeyNum -1];
                RemoveElementChar(1, Parent, Src, i, Src->KeyNum - 1);
                InsertElementChar(1, Parent, Dst, TmpKey, i + 1, 0,tno,bno,ofset);
                j++;
            }
            
        }
        
        strcpy(Parent->Key[i + 1],Dst->Key[0]);
        Parent->tableno[i + 1] = Dst->tableno[0];
        Parent->blockno[i + 1] = Dst->blockno[0];
        Parent->offset[i + 1] = Dst->offset[0];
        /* ����Ҷ�ڵ��������� */
        if (Src->KeyNum > 0)
            FindMostRightChar(Src)->Next = FindMostLeftChar(Dst);
        
    }else{
        if (Src->Children[0] != NULL){
            while (j < n) {
                Child = Src->Children[0];
                RemoveElementChar(0, Src, Child, 0, Unavailable1);
                InsertElementChar(0, Dst, Child,"-111111111", Dst->KeyNum, Unavailable1,-1,-1,-1);
                j++;
            }
            
        }else{
            while (j < n) {
                strcpy(TmpKey,Src->Key[0]);
                tno=Src->tableno[0];
                bno=Src->blockno[0];
                ofset=Src->offset[0];
                RemoveElementChar(1, Parent, Src, i, 0);
                InsertElementChar(1, Parent, Dst, TmpKey, i - 1, Dst->KeyNum,tno,bno,ofset);
                j++;
            }
            
        }
        strcpy(Parent->Key[i],Src->Key[0]);
        Parent->tableno[i] = Src->tableno[0];
        Parent->blockno[i] = Src->blockno[0];
        Parent->offset[i] = Src->offset[0];
        if (Src->KeyNum > 0)
            FindMostRightChar(Dst)->Next = FindMostLeftChar(Src);
    }  
    return Parent;
}
 
static CharBPlusTree SplitNodeChar(CharPosition Parent,CharPosition X,int i){//��ҳ��һ���ڵ������� 
    int j,k,Limit;
    CharPosition NewNode;
    
    NewNode = MallocNewCharNode();
    k = 0;
    j = X->KeyNum / 2;
    Limit = X->KeyNum;
    while (j < Limit){
        if (X->Children[0] != NULL){
            NewNode->Children[k] = X->Children[j];
            X->Children[j] = NULL;
        }
        strcpy(NewNode->Key[k],X->Key[j]);
        NewNode->tableno[k]=X->tableno[j];
        NewNode->blockno[k]=X->blockno[j];
        NewNode->offset[k]=X->offset[j];
        strcpy(X->Key[j],"-111111111");
        X->tableno[j] = -1;
        X->blockno[j] = -1;
        X->offset[j] = -1;
        NewNode->KeyNum++;X->KeyNum--;
        j++;k++;
    }
    
    if (Parent != NULL)/* �����X���Ǹ�����ô�����µĽڵ㷵��ԭ�ڵ� */
        InsertElementChar(0, Parent, NewNode, "-111111111", i + 1, Unavailable1,-1,-1,-1);
    else{
        /* �����X�Ǹ�����ô�����µĸ������� */
        Parent = MallocNewCharNode();
        InsertElementChar(0, Parent, X, "-111111111", 0, Unavailable1,-1,-1,-1);
        InsertElementChar(0, Parent, NewNode, "-111111111", 1, Unavailable1,-1,-1,-1);
        
        return Parent;
    }
    
    return X;
}
 
/* �ϲ��ڵ�,X����M/2�ؼ��֣�S�д��ڻ����M/2���ؼ���*/
static CharPosition MergeNodeChar(CharPosition Parent, CharPosition X,CharPosition S,int i){
    int Limit;
    
    /* S�Ĺؼ�����Ŀ����M/2 */
    if (S->KeyNum > LIMIT_M_2){
        /* ��S���ƶ�һ��Ԫ�ص�X�� */
        MoveElementChar(S, X, Parent, i,1);
    }else{
        /* ��Xȫ��Ԫ���ƶ���S�У�����Xɾ�� */
        Limit = X->KeyNum;
        MoveElementChar(X,S, Parent, i,Limit);
        RemoveElementChar(0, Parent, X, i, Unavailable1);
        
        free(X);
        X = NULL;
    }
    
    return Parent;
}
 
static CharBPlusTree RecursiveInsertChar(CharBPlusTree T,char *Key,int i,CharBPlusTree Parent,int tno,int bno,int ofset){//���뺯�� 
    int j,Limit;
    CharPosition Sibling;
    
    /* ���ҷ�֧ */
    j = 0;
    while (j < T->KeyNum && strcmp(Key,T->Key[j])>=0){
        /* �ظ�ֵ�����룬�ҵ�key�����λ�� */
        //if (strcmp(Key,T->Key[j])==0)
        //  return T;
        j++;
    }
    
    if (j != 0 && T->Children[0] != NULL) j--;
    //printf("%s %d\n",Key,j);
    /* ��Ҷ */
    if (T->Children[0] == NULL)
        T = InsertElementChar(1, Parent, T, Key, i, j,tno,bno,ofset);
    /* �ڲ��ڵ� */
    else
        T->Children[j] = RecursiveInsertChar(T->Children[j], Key, j, T,tno,bno,ofset);
    
    /* �����ڵ� */
    
    Limit = M;
    
    if (T->KeyNum > Limit){
        /* �� */
        if (Parent == NULL){
            /* ���ѽڵ� */
            T = SplitNodeChar(Parent, T, i);
        }
        else{
            Sibling = FindSiblingChar(Parent, i);
            if (Sibling != NULL){
                /* ��T��һ��Ԫ�أ�Key����Child���ƶ���Sibing�� */
                MoveElementChar(T, Sibling, Parent, i, 1);
            }else{
                /* ���ѽڵ� */
                T = SplitNodeChar(Parent, T, i);
            }
        }
        
    }
    
    if (Parent != NULL)
       {
	   strcpy(Parent->Key[i],T->Key[0]);
       Parent->tableno[i] = T->tableno[0];
	   Parent->blockno[i] = T->blockno[0];
	   Parent->offset[i] = T->offset[0];
	   }
    
    
    return T;
}
 
/* ���� */
extern CharBPlusTree InsertChar(CharBPlusTree T,char *Key,int tno,int bno,int ofset){
    return RecursiveInsertChar(T, Key, 0, NULL,tno,bno,ofset);
}
 
static CharBPlusTree RecursiveRemoveChar(CharBPlusTree T,char *Key,int i,CharBPlusTree Parent){
    
    int j,NeedAdjust;
    CharPosition Sibling,Tmp;
    
    Sibling = NULL;
    
    /* ���ҷ�֧ */
    j = 0;
    while (j < T->KeyNum && strcmp(Key,T->Key[j])!=-1){
        if (strcmp(Key,T->Key[j])==0)
            break;
        j++;
    }
    
    if (T->Children[0] == NULL){
        /* û�ҵ� */
        if (strcmp(Key,T->Key[j])!=0 || j == T->KeyNum)
            return T;
    }else
        if (j == T->KeyNum || strcmp(Key,T->Key[j])==-1) j--;
    
    
    
    /* ��Ҷ */
    if (T->Children[0] == NULL){
        T = RemoveElementChar(1, Parent, T, i, j);
    }else{
        T->Children[j] = RecursiveRemoveChar(T->Children[j], Key, j, T);
    }
    
    NeedAdjust = 0;
    /* ���ĸ�������һƬ��Ҷ���������������2��M֮�� */
    if (Parent == NULL && T->Children[0] != NULL && T->KeyNum < 2)
        NeedAdjust = 1;
    /* �����⣬���з���Ҷ�ڵ�Ķ�������[M/2]��M֮�䡣(����[]��ʾ����ȡ��) */
    else if (Parent != NULL && T->Children[0] != NULL && T->KeyNum < LIMIT_M_2)
        NeedAdjust = 1;
    /* ���Ǹ�����Ҷ�йؼ��ֵĸ���Ҳ��[M/2]��M֮�� */
    else if (Parent != NULL && T->Children[0] == NULL && T->KeyNum < LIMIT_M_2)
        NeedAdjust = 1;
    
    /* �����ڵ� */
    if (NeedAdjust){
        /* �� */
        if (Parent == NULL){
            if(T->Children[0] != NULL && T->KeyNum < 2){
                Tmp = T;
                T = T->Children[0];
                free(Tmp);
                return T;
            }
            
        }else{
            /* �����ֵܽڵ㣬��ؼ�����Ŀ����M/2 */
            Sibling = FindSiblingCharKeyNum_M_2(Parent, i,&j);
            if (Sibling != NULL){
                MoveElementChar(Sibling, T, Parent, j, 1);
            }else{
                if (i == 0)
                    Sibling = Parent->Children[1];
                else
                    Sibling = Parent->Children[i - 1];
                
                Parent = MergeNodeChar(Parent, T, Sibling, i);
                T = Parent->Children[i];
            }
        }
        
    }
 
    
    return T;
}
 
/* ɾ�� */
extern CharBPlusTree RemoveChar(CharBPlusTree T,char *Key){
    return RecursiveRemoveChar(T, Key, 0, NULL);
}
 
/* ���� */
extern CharBPlusTree DestroyCharTree(CharBPlusTree T){
    int i,j;
    if (T != NULL){
        i = 0;
        while (i < T->KeyNum + 1){
            DestroyCharTree(T->Children[i]);i++;
        }
        j = 0;
        free(T);
        T = NULL;
    }
    
    return T;
}
 
static void RecursiveTravelCharTree(CharBPlusTree T,int Level){
    int i;
    if (T != NULL){
        printf("  ");
        if(Level == 1)
        	printf("		");
        if(Level == 2)
        	printf("				");
        printf("[Level:%d]-->",Level);
        printf("(");
        i = 0;
        while (i < T->KeyNum)/*  T->Key[i] != Unavailable1*/
            {
			printf("%s:",T->Key[i]);
			i++;
			}
        printf(")");
        printf("\n");
        Level++;
        
        i = 0;
        while (i <= T->KeyNum) {
            RecursiveTravelCharTree(T->Children[i], Level);
            i++;
        }
        
        
    }
}
 
/* ���� */
extern void TravelCharTree(CharBPlusTree T){
	//printf("dfasdfasdfwe342123412");
    RecursiveTravelCharTree(T, 0);
    printf("\n");
}
 
/* ������Ҷ�ڵ������ */
extern void TravelCharTreeData(CharBPlusTree T){
    CharPosition Tmp;
    int i;
    if (T == NULL)
        return ;
    printf("All Data:");
    Tmp = T;
    while (Tmp->Children[0] != NULL)
        Tmp = Tmp->Children[0];
    /* ��һƬ��Ҷ */
    while (Tmp != NULL){
        i = 0;
        while (i < Tmp->KeyNum)
            printf(" %s",Tmp->Key[i++]);
        Tmp = Tmp->Next;
    }
}
void print_attr2(int TNo,char *Rec_real,char attrname[Max_Attribute_Num][20],int sum)
{
	if(DELETE_SIGN == 1){
		printf("start to delete record!\n");
		Record *R = output(Rec_real);
		deleteRecord(PTList[TNo],Buf,R->attribute[0]->value,TNo);
		printf("success to delete record!\n");
		return;
	}
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
//	printf("1111\n");
	//strcpy(projectionrecord,Project(Rec_real,c));
	Project(Rec_real,c);
	//printf("%s\n",projectionrecord);	
}
void SelectCharRecord(CharBPlusTree T,PageTable *&PT,BufferQue* &Buf,int ofset,char attrname[Max_Attribute_Num][20],int sum)//��TNo�����һ����¼
{
	int Tno=T->tableno[ofset];
	int Bno=T->blockno[ofset];
	int	offset=T->offset[ofset];
	//�����Ƿ��ڻ�����
	PageNode *p1 = selectBuf_Block(Buf,Bno,Tno);
	if(p1){//p1�ǿգ��������ڻ������пɵĿ�Bno 
		printf("��%s��%d�ڻ�����ҳ��\n",TableList[Tno],Bno);
		Block *c=p1->block;
		printf("%s\n",c->Re[offset]);
		print_attr2(Tno,c->Re[offset],attrname,sum);
		/////////////////////////////////////////// 
	}
	else{
		Page *p2 = selectPT_Block(PT,Bno);
		if(p2){//p2�ǿգ�����ڵ�δ�ڻ����� 
			Block *b=FIFO(PT,Buf,p2);
			printf("%s\n",b->Re[offset]);
			print_attr2(Tno,b->Re[offset],attrname,sum);
		}
		else
			printf("��%d���Ҳ�����%d\n",Tno,Bno);
	}
	return;
}
CharBPlusTree search(CharBPlusTree T,char *key,int &offset)
{   //printf("4\n");
//	TravelCharTree(T);
	while(T->Children[0]!=NULL)
	{
		for(int i=0;i<T->KeyNum;i++)
		{	
		//	printf("%d %d->",strcmp(key,T->Key[i]),strcmp(key,T->Key[i+1]));	
			if(strcmp(key,T->Key[i])>=0)
			{
				if(i==T->KeyNum-1)
				{
				//	printf("%s %d\n",T->Key[i],i);
					T=T->Children[i];
					
					break;
				}
				else if(strcmp(key,T->Key[i+1])<=0)
				{
				//	printf("%s %d\n",T->Key[i],i);	
					T=T->Children[i];		
					break;
				}
			}
		}
	}
//	printf("5\n");
	//printf("") 
	for(int j=0;j<T->KeyNum;j++)
	{
	//	printf("%s %s\n",key,T->Key[j]);
		if(strcmp(key,T->Key[j])==0)
		{
	//	printf("find key\n");
		offset =j;
		return T;
		}
	}
	T=T->Next;
	if(T==NULL)
	{
		return NULL;
	}
	for(int j=0;j<T->KeyNum;j++)
	{
		if(strcmp(key,T->Key[j])==0)
		{
	//	printf("find key\n");
		offset =j;
		return T;
		}
	}
//	printf("cannot find\n"); 
	return NULL;
}
void SEARCH(CharBPlusTree T,char *key,int offset,PageTable *&PT,BufferQue* &Buf,char attrname[Max_Attribute_Num][20],int sum)
{
//	printf("4444\n");
	T=search(T,key,offset);
//	printf("5555\n");
	int i=offset;
	i=offset;
//	printf("%s %d\n",T->Key[i],i);
	while(1)
	{
		if(strcmp(T->Key[i],"-111111111")==0||i==M)	
		{
			T=T->Next;
			i=0;
			continue;
		}
		else
		{
			if(strcmp(key,T->Key[i])!=0)
			break;
		//	printf("find 1\n");
//			printf("66666\n");
			SelectCharRecord(T,PT,Buf,i,attrname,sum);
//			printf("77777\n");
		}
		i++;
	}
	return;
}
void SEARCH1(CharBPlusTree T,char *key,PageTable *&PT,BufferQue* &Buf,char attrname[Max_Attribute_Num][20],int sum)
{
	int offset=0;
	T=search(T,key,offset);
	int i=offset;
	i=offset;
	CharBPlusTree Tmp;    
    Tmp = MallocNewCharNode();
    Tmp = T;
        while (i < Tmp->KeyNum){
        	if(strcmp(Tmp->Key[i],key)>0)
			SelectCharRecord(Tmp,PT,Buf,i,attrname,sum);
        	i++;
        } 
	Tmp = Tmp->Next;
    while (Tmp != NULL){
        i = 0;
        while (i < Tmp->KeyNum){
        	if(strcmp(Tmp->Key[i],key)>0)
			SelectCharRecord(Tmp,PT,Buf,i,attrname,sum);
        	i++;
        }            
        Tmp = Tmp->Next;
    }	
}
void SEARCH2(CharBPlusTree T,char *key,PageTable *&PT,BufferQue* &Buf,char attrname[Max_Attribute_Num][20],int sum)
{	
	int j;
	CharBPlusTree Tmp;    
    Tmp = MallocNewCharNode();
    Tmp=T;
    while (Tmp->Children[0] != NULL)
        Tmp = Tmp->Children[0];
    while (Tmp != NULL){
        j = 0;
        while ( j< Tmp->KeyNum){
			if(strcmp(Tmp->Key[j],key)<0)
				SelectCharRecord(Tmp,PT,Buf,j,attrname,sum);
			else if(strcmp(Tmp->Key[j],key)==0)
				return; 	
        	j++;
        }            
        Tmp = Tmp->Next;
    }		
}
void SEARCH3(CharBPlusTree T,char *key,PageTable *&PT,BufferQue* &Buf,char attrname[Max_Attribute_Num][20],int sum)
{	
	int j;
	CharBPlusTree Tmp;    
    Tmp = MallocNewCharNode();
    Tmp=T;
    while (Tmp->Children[0] != NULL)
        Tmp = Tmp->Children[0];
    while (Tmp != NULL){
        j = 0;
        while ( j< Tmp->KeyNum){
			if(strcmp(Tmp->Key[j],key)!=0)
				SelectCharRecord(Tmp,PT,Buf,j,attrname,sum);	
        	j++;
        }            
        Tmp = Tmp->Next;
    }		
}
/*
int main() {
    int i;
    int offset=0;
    CharBPlusTree T;
    T = InitializeCharTree();
    T=InsertChar(T,"asdwasdadf",1,2,3);
    T=InsertChar(T,"sdwsadf",1,2,3);
    T=InsertChar(T,"asdadf",1,2,3);
    T=InsertChar(T,"fsdsadasfadf",1,2,3);
    T=InsertChar(T,"fasdf",1,2,3);
    T=InsertChar(T,"fsdasfaasdasddf",1,2,3);
    T=InsertChar(T,"fsdasdasddf",1,2,3);
    T=InsertChar(T,"dasfaasdf",1,2,3);
    T=InsertChar(T,"dasfaasd",1,2,3);
    T=InsertChar(T,"sfaasdasddf",1,2,3);
        T=InsertChar(T,"asdwasdadf",1,2,3);
    T=InsertChar(T,"sdwsadf",1,2,3);
    T=InsertChar(T,"asdadf",1,2,3);
    T=InsertChar(T,"fsdsadasfadf",1,2,3);
    T=InsertChar(T,"fasdf",1,2,3);
    T=InsertChar(T,"fsdasfaasdasddf",1,2,3);
    T=InsertChar(T,"fsdasdasddf",1,2,3);
    T=InsertChar(T,"dasfaasdf",1,2,3);
    T=InsertChar(T,"dasfaasd",1,2,3);
    T=InsertChar(T,"sfaasdasddf",1,2,3);
        T=InsertChar(T,"asdwasdadf",1,2,3);
    T=InsertChar(T,"sdwsadf",1,2,3);
    T=InsertChar(T,"asdadf",1,2,3);
    T=InsertChar(T,"fsdsadasfadf",1,2,3);
    T=InsertChar(T,"fasdf",1,2,3);
    T=InsertChar(T,"fsdasfaasdasddf",1,2,3);
    T=InsertChar(T,"fsdasdasddf",1,2,3);
    T=InsertChar(T,"dasfaasdf",1,2,3);
    T=InsertChar(T,"dasfaasdf",1,2,3);
        T=InsertChar(T,"dasfaasdf",1,2,3);
            T=InsertChar(T,"dasfaasdf",1,2,3);
    T=InsertChar(T,"dasfaasd",1,2,3);
    T=InsertChar(T,"sfaasdasddf",1,2,3);
	TravelCharTree(T);
	
   // for(int j=0;j<16;j++)
   //     printf("%c ",i+j); 
	SEARCH(T,"dasfaasdf",offset);
//	printf("%d",s->Key[offset]);
    DestroyCharTree(T);
}
*/
/*int SaveCharBPlusNode(CharBPlusTree BTNode,int TNo,int Level0,int Level1,int Level2)
{
	FILE *f;
	char filename[20];
	if(Level1 == -1&&Level2 == -1){
		sprintf(filename,"%d",Level0);
	}
	else if(Level2 == -1){
		sprintf(filename,"%d_%d",Level0,Level1);
	}
	else{
		sprintf(filename,"%d_%d_%d",Level0,Level1,Level2);
	}
	char s[255]; 
    sprintf(s, "%s\\CharBPlusTree\\%s",TableList[TNo],filename);
    remove(s);
	sprintf(s, "%s\\CharBPlusTree\\%s",TableList[TNo],filename);
	//printf("%s\n",s);
	f = fopen(s,"w+");
    fclose(f);
    f = fopen(s,"rb+");
	if(f != NULL)
	{ 
		//printf("���ļ�\n");
		//int i = BTNode->KeyNum;
		fwrite(&(BTNode->KeyNum), sizeof(int), 1, f);
        //fseek(f, sizeof(P->PageID), 0);
        fwrite(&(BTNode->Key), sizeof(BTNode->Key), 1, f);
		fwrite(&(BTNode->tableno), sizeof(BTNode->tableno), 1, f);
        //fseek(f, sizeof(P->BlockID), 0);
        fwrite(&(BTNode->blockno), sizeof(BTNode->blockno), 1, f);
        //fseek(f, sizeof(P->CreateBlock), 0);
        fwrite(&(BTNode->offset), sizeof(BTNode->offset), 1, f);
        fclose(f);
        //printf("����ɹ�\n");
        return 0;
    }
}
int SaveCharBPlusTree(CharBPlusTree T,int TNo)
{
	SaveCharBPlusNode(T,TNo,0,-1,-1);
	if(T->Children[0] == NULL)
		return 0;
	//printf("%d\n",T->KeyNum);
	CharBPlusTree T1 = MallocNewNode();
	CharBPlusTree T2 = MallocNewNode();
	for(int level1 = 0;level1 <T->KeyNum;level1++)
	{
		T1 = T->Children[level1];
		SaveCharBPlusNode(T1,TNo,0,level1,-1);
		//printf("%d\n",T1->KeyNum);
		for(int level2 = 0;level2 <T1->KeyNum;level2++)
		{
			T2 = T1->Children[level2];
			if(T2 != NULL)
			{
				SaveCharBPlusNode(T2,TNo,0,level1,level2);
				free(T2);
				T2 = MallocNewNode();
			} 
				
		}
		free(T1);
		T1 = MallocNewNode();
	}

	return 0;
}
CharBPlusTree ReadCharBPlusNode(int TNo,int Level0,int Level1,int Level2)
{
	FILE *f;
	char filename[20];
	if(Level1 == -1&&Level2 == -1){
		sprintf(filename,"%d",Level0);
	}
	else if(Level2 == -1){
		sprintf(filename,"%d_%d",Level0,Level1);
	}
	else{
		sprintf(filename,"%d_%d_%d",Level0,Level1,Level2);
	}
	char s[255]; 
    sprintf(s, "%s\\CharBPlusTree\\%s",TableList[TNo],filename);
	CharBPlusTree BTNode = MallocNewNode();
	f = fopen(s, "rb+");//��rb+��ʽ�򿪱�������ļ����������
    if(f == NULL)
    {
	   return NULL;
	}
	if(f != NULL)
	{ 
        fread(&(BTNode->KeyNum), sizeof(int), 1, f);
        //fseek(f, sizeof(P->PageID), 0);
        fread(&(BTNode->Key), sizeof(BTNode->Key), 1, f);
		fread(&(BTNode->tableno), sizeof(BTNode->tableno), 1, f);
        //fseek(f, sizeof(P->BlockID), 0);
        fread(&(BTNode->blockno), sizeof(BTNode->blockno), 1, f);
        //fseek(f, sizeof(P->CreateBlock), 0);
        fread(&(BTNode->offset), sizeof(BTNode->offset), 1, f);
        fclose(f);
        return BTNode;
    }
}
CharBPlusTree ReadCharBPlusTree(int TNo)
{
	CharBPlusTree T = ReadCharBPlusNode(TNo,0,-1,-1);
	//printf("%d\n",T->KeyNum);
	for(int level1 = 0;level1 <T->KeyNum;level1++)
	{
		CharBPlusTree T1 = ReadCharBPlusNode(TNo,0,level1,-1);
		T->Children[level1] = T1;
		if(level1>0)
			T->Children[level1-1]->Next = T1;
		if(T1 == NULL)
			break;
		for(int level2 = 0;level2 <T1->KeyNum;level2++)
		{
			CharBPlusTree T2 = ReadCharBPlusNode(TNo,0,level1,level2);
			T1->Children[level2] = T2;
			if(level2>0)
				T1->Children[level2-1]->Next = T2; 
			if(T2 == NULL)
				break;				
		}

	}

	return T;
}*/
