#pragma once 
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "table.cpp"
#include"projection.cpp"
#include"select.cpp"
#include "delete.cpp"
typedef struct BPlusNode *BPlusTree,*Position;
typedef int KeyType;
/*B+���Ľڵ�ṹ*/ 
struct BPlusNode{
    int KeyNum;//��ֵ���� 
    KeyType Key[M + 1];//��ֵ���� 
    int tableno[M + 1];
	int blockno[M + 1];
	int offset[M + 1]; 
    BPlusTree Children[M + 1];//�ӽڵ� 
    BPlusTree Next;//�ֵܽڵ� 
};
 
static KeyType Unavailable = INT_MIN;
 
/* ���ɽڵ㲢��ʼ�� */
static BPlusTree MallocNewNode(){
    BPlusTree NewNode;
    int i;
    NewNode = (BPlusNode*)malloc(sizeof(BPlusNode));
    if (NewNode == NULL)
        exit(EXIT_FAILURE);
    i = 0;
    while (i < M + 1){
        NewNode->Key[i] = Unavailable;
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
extern BPlusTree Initialize(){
    
    BPlusTree T;    
    if (M < (3)){
        printf("M��С����3��");
        exit(EXIT_FAILURE);
        
    }
    /* ����� */
    T = MallocNewNode();
    
    return T;
}
/*�ҵ�����ߵ�Ҷ�ӽڵ�*/ 
static Position FindMostLeft(Position P){
    Position Tmp;
    Tmp = P;
    while (Tmp != NULL && Tmp->Children[0] != NULL) {
        Tmp = Tmp->Children[0];
    }
    return Tmp;
}
/*�ҵ����ұߵ�Ҷ�ӽڵ�*/ 
static Position FindMostRight(Position P){
    Position Tmp;
    Tmp = P;
    while (Tmp != NULL && Tmp->Children[Tmp->KeyNum - 1] != NULL) {
        Tmp = Tmp->Children[Tmp->KeyNum - 1];
    }
    return Tmp;
}
/*Ѱ��parent��children[i]��һ���ֵܽڵ㣬��洢�Ĺؼ���δ�������򷵻�NULL */
static Position FindSibling(Position Parent,int i){
    Position Sibling;
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
static Position FindSiblingKeyNum_M_2(Position Parent,int i,int *j){
    int Limit;
    Position Sibling;
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
static Position InsertElement(int isKey, Position Parent,Position X,KeyType Key,int i,int j,int tno,int bno,int ofset){
    
    int k;
    if (isKey){
        /* ����key */
        k = X->KeyNum - 1;
        while (k >= j){
            X->Key[k + 1] = X->Key[k];
			X->tableno[k + 1] = X->tableno[k];
			X->blockno[k + 1] = X->blockno[k];
			X->offset[k + 1] = X->offset[k];
			k--;
        }
        X->Key[j] = Key; 
        X->tableno[j]=tno;
        X->blockno[j]=bno;
        X->offset[j]=ofset;
        if (Parent != NULL)
            {
			Parent->Key[i] = X->Key[0];
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
            Parent->Key[k + 1] = Parent->Key[k];
            Parent->tableno[k + 1] = Parent->tableno[k];
            Parent->blockno[k + 1] = Parent->blockno[k];
            Parent->offset[k + 1] = Parent->offset[k];
            k--;
        }
        Parent->Key[i] = X->Key[0];
        Parent->tableno[i] = X->tableno[0];
        Parent->blockno[i] = X->blockno[0];
        Parent->offset[i] = X->offset[0];
        Parent->Children[i] = X;
        Parent->KeyNum++;   
    }
    return X;
}
 
/*��Ҫ��X�Ƴ�Key��ʱ��i��X��Parent��λ�ã�j��KeyҪ�Ƴ���λ��
   ��Ҫ��Parent�Ƴ�X�ڵ��ʱ��i��Ҫ�Ƴ���λ�ã�Key��j��ֵû����
*/ 
static Position RemoveElement(int isKey, Position Parent,Position X,int i,int j){
    
    int k,Limit;
    
    if (isKey){
        Limit = X->KeyNum;
        /* ɾ��key */
        k = j + 1;
        while (k < Limit){
            X->Key[k - 1] = X->Key[k];
			X->tableno[k - 1] = X->tableno[k];
			X->blockno[k - 1] = X->blockno[k];
			X->offset[k - 1] = X->offset[k];
			k++;
        }
        
        X->Key[X->KeyNum - 1] = Unavailable;
        X->tableno[X->KeyNum - 1]=-1;
        X->blockno[X->KeyNum - 1]=-1;
        X->offset[X->KeyNum - 1]=-1;
        Parent->Key[i] = X->Key[0];
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
            Parent->Key[k - 1] = Parent->Key[k];
            Parent->tableno[k - 1] = Parent->tableno[k];
            Parent->blockno[k - 1] = Parent->blockno[k];
            Parent->offset[k - 1] = Parent->offset[k];
            k++;
        }
        Parent->Children[Parent->KeyNum - 1] = NULL;
        Parent->Key[Parent->KeyNum - 1] = Unavailable;
        Parent->tableno[Parent->KeyNum - 1] = -1;
        Parent->blockno[Parent->KeyNum - 1] = -1;
        Parent->offset[Parent->KeyNum - 1] = -1;
        Parent->KeyNum--;
    }
    return X;
}
 
/* Src��Dst���������ڵĽڵ㣬i��Src��Parent�е�λ�ã�
 ��Src��Ԫ���ƶ���Dst�� ,n���ƶ�Ԫ�صĸ���*/
static Position MoveElement(Position Src,Position Dst,Position Parent,int i,int n){
    KeyType TmpKey;
    Position Child;
    int j,SrcInFront;
    int tno,bno,ofset;
    SrcInFront = 0;
    
    if (Src->Key[0] < Dst->Key[0])
        SrcInFront = 1;
    
    j = 0;
    /* �ڵ�Src��Dstǰ�� */
    if (SrcInFront){
        if (Src->Children[0] != NULL)//Src����Ҷ�ӽڵ� 
		{
            while (j < n) {
                Child = Src->Children[Src->KeyNum - 1];//child��src���һ���ӽڵ� 
                RemoveElement(0, Src, Child, Src->KeyNum - 1, Unavailable);//�Ƴ�src��child 
                InsertElement(0, Dst, Child, Unavailable, 0, Unavailable,-1,-1,-1);//��child����dst 
                j++;
            }
        }else{
            while (j < n) {//Ҷ�ӽڵ�ı丳ֵ��� 
                TmpKey = Src->Key[Src->KeyNum -1];
                tno=Src->tableno[Src->KeyNum -1];
                bno=Src->blockno[Src->KeyNum -1];
                ofset=Src->offset[Src->KeyNum -1];
                RemoveElement(1, Parent, Src, i, Src->KeyNum - 1);
                InsertElement(1, Parent, Dst, TmpKey, i + 1, 0,tno,bno,ofset);
                j++;
            }
            
        }
        
        Parent->Key[i + 1] = Dst->Key[0];
        Parent->tableno[i + 1] = Dst->tableno[0];
        Parent->blockno[i + 1] = Dst->blockno[0];
        Parent->offset[i + 1] = Dst->offset[0];
        /* ����Ҷ�ڵ��������� */
        if (Src->KeyNum > 0)
            FindMostRight(Src)->Next = FindMostLeft(Dst);
        
    }else{
        if (Src->Children[0] != NULL){
            while (j < n) {
                Child = Src->Children[0];
                RemoveElement(0, Src, Child, 0, Unavailable);
                InsertElement(0, Dst, Child, Unavailable, Dst->KeyNum, Unavailable,-1,-1,-1);
                j++;
            }
            
        }else{
            while (j < n) {
                TmpKey = Src->Key[0];
                tno=Src->tableno[0];
                bno=Src->blockno[0];
                ofset=Src->offset[0];
                RemoveElement(1, Parent, Src, i, 0);
                InsertElement(1, Parent, Dst, TmpKey, i - 1, Dst->KeyNum,tno,bno,ofset);
                j++;
            }
            
        }
        
        Parent->Key[i] = Src->Key[0];
        Parent->tableno[i] = Src->tableno[0];
        Parent->blockno[i] = Src->blockno[0];
        Parent->offset[i] = Src->offset[0];
        if (Src->KeyNum > 0)
            FindMostRight(Dst)->Next = FindMostLeft(Src);
    }  
    return Parent;
}
 
static BPlusTree SplitNode(Position Parent,Position X,int i){//��ҳ��һ���ڵ������� 
    int j,k,Limit;
    Position NewNode;
    
    NewNode = MallocNewNode();
    
    k = 0;
    j = X->KeyNum / 2;
    Limit = X->KeyNum;
    while (j < Limit){
        if (X->Children[0] != NULL){
            NewNode->Children[k] = X->Children[j];
            X->Children[j] = NULL;
        }
        NewNode->Key[k] = X->Key[j];
        NewNode->tableno[k]=X->tableno[j];
        NewNode->blockno[k]=X->blockno[j];
        NewNode->offset[k]=X->offset[j];
        X->Key[j] = Unavailable;
        X->tableno[j] = -1;
        X->blockno[j] = -1;
        X->offset[j] = -1;
        NewNode->KeyNum++;X->KeyNum--;
        j++;k++;
    }
    
    if (Parent != NULL)/* �����X���Ǹ�����ô�����µĽڵ㷵��ԭ�ڵ� */
        InsertElement(0, Parent, NewNode, Unavailable, i + 1, Unavailable,-1,-1,-1);
    else{
        /* �����X�Ǹ�����ô�����µĸ������� */
        Parent = MallocNewNode();
        InsertElement(0, Parent, X, Unavailable, 0, Unavailable,-1,-1,-1);
        InsertElement(0, Parent, NewNode, Unavailable, 1, Unavailable,-1,-1,-1);
        
        return Parent;
    }
    
    return X;
}
 
/* �ϲ��ڵ�,X����M/2�ؼ��֣�S�д��ڻ����M/2���ؼ���*/
static Position MergeNode(Position Parent, Position X,Position S,int i){
    int Limit;
    
    /* S�Ĺؼ�����Ŀ����M/2 */
    if (S->KeyNum > LIMIT_M_2){
        /* ��S���ƶ�һ��Ԫ�ص�X�� */
        MoveElement(S, X, Parent, i,1);
    }else{
        /* ��Xȫ��Ԫ���ƶ���S�У�����Xɾ�� */
        Limit = X->KeyNum;
        MoveElement(X,S, Parent, i,Limit);
        RemoveElement(0, Parent, X, i, Unavailable);
        
        free(X);
        X = NULL;
    }
    
    return Parent;
}
 
static BPlusTree RecursiveInsert(BPlusTree T,KeyType Key,int i,BPlusTree Parent,int tno,int bno,int ofset){//���뺯�� 
    int j,Limit;
    Position Sibling;
    
    /* ���ҷ�֧ */
    j = 0;
    while (j < T->KeyNum && Key >= T->Key[j]){
        /* �ظ�ֵ�����룬�ҵ�key�����λ�� */
        j++;
    }
    if (j != 0 && T->Children[0] != NULL) j--;
    
    /* ��Ҷ */
    if (T->Children[0] == NULL)
        T = InsertElement(1, Parent, T, Key, i, j,tno,bno,ofset);
    /* �ڲ��ڵ� */
    else
        T->Children[j] = RecursiveInsert(T->Children[j], Key, j, T,tno,bno,ofset);
    
    /* �����ڵ� */
    
    Limit = M;
    
    if (T->KeyNum > Limit){
        /* �� */
        if (Parent == NULL){
            /* ���ѽڵ� */
            T = SplitNode(Parent, T, i);
        }
        else{
            Sibling = FindSibling(Parent, i);
            if (Sibling != NULL){
                /* ��T��һ��Ԫ�أ�Key����Child���ƶ���Sibing�� */
                MoveElement(T, Sibling, Parent, i, 1);
            }else{
                /* ���ѽڵ� */
                T = SplitNode(Parent, T, i);
            }
        }
        
    }
    
    if (Parent != NULL)
        {
		Parent->Key[i] = T->Key[0];
		Parent->tableno[i] = T->tableno[0];
		Parent->blockno[i] = T->blockno[0];
		Parent->offset[i] = T->offset[0];
	    }
    
    
    return T;
}
 
/* ���� */
extern BPlusTree Insert(BPlusTree T,KeyType Key,int tno,int bno,int ofset){
    return RecursiveInsert(T, Key, 0, NULL,tno,bno,ofset);
}
 
static BPlusTree RecursiveRemove(BPlusTree T,KeyType Key,int i,BPlusTree Parent){
    
    int j,NeedAdjust;
    Position Sibling,Tmp;
    
    Sibling = NULL;
    
    /* ���ҷ�֧ */
    j = 0;
    while (j < T->KeyNum && Key >= T->Key[j]){
        if (Key == T->Key[j])
            break;
        j++;
    }
    
    if (T->Children[0] == NULL){
        /* û�ҵ� */
        if (Key != T->Key[j] || j == T->KeyNum)
            return T;
    }else
        if (j == T->KeyNum || Key < T->Key[j]) j--;
    
    
    
    /* ��Ҷ */
    if (T->Children[0] == NULL){
        T = RemoveElement(1, Parent, T, i, j);
    }else{
        T->Children[j] = RecursiveRemove(T->Children[j], Key, j, T);
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
            Sibling = FindSiblingKeyNum_M_2(Parent, i,&j);
            if (Sibling != NULL){
                MoveElement(Sibling, T, Parent, j, 1);
            }else{
                if (i == 0)
                    Sibling = Parent->Children[1];
                else
                    Sibling = Parent->Children[i - 1];
                
                Parent = MergeNode(Parent, T, Sibling, i);
                T = Parent->Children[i];
            }
        }
        
    }
 
    
    return T;
}
 
/* ɾ�� */
extern BPlusTree Remove(BPlusTree T,KeyType Key){
    return RecursiveRemove(T, Key, 0, NULL);
}
 
/* ���� */
extern BPlusTree Destroy(BPlusTree T){
    int i,j;
    if (T != NULL){
        i = 0;
        while (i < T->KeyNum + 1){
            Destroy(T->Children[i]);i++;
        }
        j = 0;
        free(T);
        T = NULL;
    }
    
    return T;
}
 
static void RecursiveTravel(BPlusTree T,int Level){
    int i;
    if (T != NULL){
        printf("\n");
        if(Level == 1)
        	printf("		");
        if(Level == 2)
        	printf("				");
        printf("[Level:%d]-->",Level);
        printf("(");
        i = 0;
        while (i < T->KeyNum)/*  T->Key[i] != Unavailable*/
            {
			printf("%d: ",T->Key[i]);
			i++;
			}
        printf(")");
        
        Level++;
        
        i = 0;
        while (i <= T->KeyNum) {
            RecursiveTravel(T->Children[i], Level);
            i++;
        }
        
        
    }
}
 
/* ���� */
extern void Travel(BPlusTree T){
    RecursiveTravel(T, 0);
    printf("\n");
}
 
/* ������Ҷ�ڵ������ */
extern void TravelData(BPlusTree T){
    Position Tmp;
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
            {
			printf(" %d %d %d %d",Tmp->Key[i],Tmp->tableno[i],Tmp->blockno[i],Tmp->offset[i]);
            i++;
   		    }
        Tmp = Tmp->Next;
    }
}
int SaveBPlusNode(BPlusTree BTNode,int TNo,int Level0,int Level1,int Level2)
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
    sprintf(s, "%s\\BPlusTree\\%s",TableList[TNo],filename);
    remove(s);
	sprintf(s, "%s\\BPlusTree\\%s",TableList[TNo],filename);
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
int SaveBPlusTree(BPlusTree T,int TNo)
{
	SaveBPlusNode(T,TNo,0,-1,-1);
	if(T->Children[0] == NULL)
		return 0;
	//printf("%d\n",T->KeyNum);
	BPlusTree T1 = MallocNewNode();
	BPlusTree T2 = MallocNewNode();
	for(int level1 = 0;level1 <T->KeyNum;level1++)
	{
		T1 = T->Children[level1];
		SaveBPlusNode(T1,TNo,0,level1,-1);
		//printf("%d\n",T1->KeyNum);
		for(int level2 = 0;level2 <T1->KeyNum;level2++)
		{
			T2 = T1->Children[level2];
			if(T2 != NULL)
			{
				SaveBPlusNode(T2,TNo,0,level1,level2);
				free(T2);
				T2 = MallocNewNode();
			} 
				
		}
		free(T1);
		T1 = MallocNewNode();
	}

	return 0;
}
BPlusTree ReadBPlusNode(int TNo,int Level0,int Level1,int Level2)
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
    sprintf(s, "%s\\BPlusTree\\%s",TableList[TNo],filename);
	BPlusTree BTNode = MallocNewNode();
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
BPlusTree ReadBPlusTree(int TNo)
{
	BPlusTree T = ReadBPlusNode(TNo,0,-1,-1);
	//printf("%d\n",T->KeyNum);
	for(int level1 = 0;level1 <T->KeyNum;level1++)
	{
		BPlusTree T1 = ReadBPlusNode(TNo,0,level1,-1);
		T->Children[level1] = T1;
		if(level1>0)
			T->Children[level1-1]->Next = T1;
		if(T1 == NULL)
			break;
		for(int level2 = 0;level2 <T1->KeyNum;level2++)
		{
			BPlusTree T2 = ReadBPlusNode(TNo,0,level1,level2);
			T1->Children[level2] = T2;
			if(level2>0)
				T1->Children[level2-1]->Next = T2; 
			if(T2 == NULL)
				break;				
		}

	}

	return T;
}
void print_attrint(int TNo,char *Rec_real,char attrname[Max_Attribute_Num][20],int sum)
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
	Project(Rec_real,c);
	//strcpy(projectionrecord,Project(Rec_real,c));
	//printf("%s\n",projectionrecord);
	return;
}
void SelectRecord(BPlusTree &BT,PageTable *&PT,BufferQue* &Buf,int ofset,char attrname[Max_Attribute_Num][20],int sum)//��TNo�����һ����¼
{				//CharBPlusTree T,PageTable *&PT,BufferQue* &Buf,int ofset,char attrname[Max_Attribute_Num][20],int sum
	int Tno=BT->tableno[ofset];
	int Bno=BT->blockno[ofset];
	int	offset=BT->offset[ofset];
	//�����Ƿ��ڻ�����
	PageNode *p1 = selectBuf_Block(Buf,Bno,Tno);
	if(p1){//p1�ǿգ��������ڻ������пɵĿ�Bno 
	//	printf("��%s��%d�ڻ�����ҳ��\n",TableList[TNo],Bno);
		Block *c=p1->block;
		print_attrint(Tno,c->Re[offset],attrname,sum);
		/////////////////////////////////////////// 
	}
	else{
		Page *p2 = selectPT_Block(PT,Bno);
		if(p2){//p2�ǿգ�����ڵ�δ�ڻ����� 

			Block *b=FIFO(PT,Buf,p2); 
			print_attrint(Tno,b->Re[offset],attrname,sum);
		}
		else
			printf("��%d���Ҳ�����%d\n",Tno,Bno);
	}
	return ;
}
BPlusTree searchint(BPlusTree T,int key,int &offset)
{   //printf("4\n");
//	TravelCharTree(T);
	while(T->Children[0]!=NULL)
	{
		for(int i=0;i<T->KeyNum;i++)
		{	
		//	printf("%d %d->",strcmp(key,T->Key[i]),strcmp(key,T->Key[i+1]));	
			if(key>=T->Key[i])
			{
				if(i==T->KeyNum-1)
				{
				//	printf("%s %d\n",T->Key[i],i);
					T=T->Children[i];
					
					break;
				}
				else if(key<=T->Key[i+1])
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
		if(key==T->Key[j])
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
		if(key==T->Key[j])
		{
	//	printf("find key\n");
		offset =j;
		return T;
		}
	}
//	printf("cannot find\n"); 
	return NULL;
}
void SEARCHINT(BPlusTree T,int key,int offset,PageTable *&PT,BufferQue* &Buf,char attrname[Max_Attribute_Num][20],int sum)
{
//	printf("4444\n");
	T=searchint(T,key,offset);
//	printf("5555\n");
	int i=offset;
	i=offset;
//	printf("%s %d\n",T->Key[i],i);
	while(1)
	{
		if(T->Key[i]==Unavailable||i==M)	
		{
			T=T->Next;
			i=0;
			continue;
		}
		else
		{
			if(key!=T->Key[i])
			break;
		//	printf("find 1\n");
//			printf("66666\n");
			SelectRecord(T,PT,Buf,i,attrname,sum);
//			printf("77777\n");
		}
		i++;
	}
	return;
}
void SEARCHINT1(BPlusTree T,int key,PageTable *&PT,BufferQue* &Buf,char attrname[Max_Attribute_Num][20],int sum)
{
	int offset=0;
	T=searchint(T,key,offset);
	int i=offset;
	i=offset;
	BPlusTree Tmp;    
    Tmp = MallocNewNode();
    Tmp = T;
        while (i < Tmp->KeyNum){
        	if(Tmp->Key[i]>key)
			SelectRecord(Tmp,PT,Buf,i,attrname,sum);
        	i++;
        } 
	Tmp = Tmp->Next;
    while (Tmp != NULL){
        i = 0;
        while (i < Tmp->KeyNum){
        	if(Tmp->Key[i]>key)
			SelectRecord(Tmp,PT,Buf,i,attrname,sum);
        	i++;
        }            
        Tmp = Tmp->Next;
    }	
}
void SEARCHINT2(BPlusTree T,int key,PageTable *&PT,BufferQue* &Buf,char attrname[Max_Attribute_Num][20],int sum)
{	
	int j;
	BPlusTree Tmp;    
    Tmp = MallocNewNode();
    Tmp=T;
    while (Tmp->Children[0] != NULL)
        Tmp = Tmp->Children[0];
    while (Tmp != NULL){
        j = 0;
        while ( j< Tmp->KeyNum){
			if(Tmp->Key[j]<key)
				SelectRecord(Tmp,PT,Buf,j,attrname,sum);
			else if(Tmp->Key[j]==key)
				return; 	
        	j++;
        }            
        Tmp = Tmp->Next;
    }		
}
void SEARCHINT3(BPlusTree T,int key,PageTable *&PT,BufferQue* &Buf,char attrname[Max_Attribute_Num][20],int sum)
{	
	int j;
	BPlusTree Tmp;    
    Tmp = MallocNewNode();
    Tmp=T;
    while (Tmp->Children[0] != NULL)
        Tmp = Tmp->Children[0];
    while (Tmp != NULL){
        j = 0;
        while ( j< Tmp->KeyNum){
			if(Tmp->Key[j]!=key)
				SelectRecord(Tmp,PT,Buf,j,attrname,sum);	
        	j++;
        }            
        Tmp = Tmp->Next;
    }		
}
/*
int main() {
    int i;
    int offset=0;
   	BPlusTree T;
    T = Initialize();
	T=Insert(T,1,0,0,0);
	T=Insert(T,2,0,0,0);
	T=Insert(T,3,0,0,0);
	T=Insert(T,4,0,0,0);
		T=Insert(T,1,0,0,0);
	T=Insert(T,2,0,0,0);
	T=Insert(T,3,0,0,0);
	T=Insert(T,4,0,0,0);
		T=Insert(T,1,0,0,0);
	T=Insert(T,2,0,0,0);
	T=Insert(T,3,0,0,0);
	T=Insert(T,4,0,0,0);
		T=Insert(T,1,0,0,0);
	T=Insert(T,2,0,0,0);
	T=Insert(T,3,0,0,0);
	T=Insert(T,4,0,0,0);
	T=Insert(T,3,0,0,0);
	T=Insert(T,4,0,0,0);
	Travel(T);
	
   // for(int j=0;j<16;j++)
   //     printf("%c ",i+j); 
//	SEARCH(T,"dasfaasdf",offset);
//	printf("%d",s->Key[offset]);
    Destroy(T);
}
*/
