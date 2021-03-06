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
/*B+树的节点结构*/ 
struct BPlusNode{
    int KeyNum;//键值个数 
    KeyType Key[M + 1];//键值数组 
    int tableno[M + 1];
	int blockno[M + 1];
	int offset[M + 1]; 
    BPlusTree Children[M + 1];//子节点 
    BPlusTree Next;//兄弟节点 
};
 
static KeyType Unavailable = INT_MIN;
 
/* 生成节点并初始化 */
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
 
/* 初始化 */
extern BPlusTree Initialize(){
    
    BPlusTree T;    
    if (M < (3)){
        printf("M最小等于3！");
        exit(EXIT_FAILURE);
        
    }
    /* 根结点 */
    T = MallocNewNode();
    
    return T;
}
/*找到最左边的叶子节点*/ 
static Position FindMostLeft(Position P){
    Position Tmp;
    Tmp = P;
    while (Tmp != NULL && Tmp->Children[0] != NULL) {
        Tmp = Tmp->Children[0];
    }
    return Tmp;
}
/*找到最右边的叶子节点*/ 
static Position FindMostRight(Position P){
    Position Tmp;
    Tmp = P;
    while (Tmp != NULL && Tmp->Children[Tmp->KeyNum - 1] != NULL) {
        Tmp = Tmp->Children[Tmp->KeyNum - 1];
    }
    return Tmp;
}
/*寻找parent的children[i]的一个兄弟节点，其存储的关键字未满，否则返回NULL */
static Position FindSibling(Position Parent,int i){
    Position Sibling;
    int Limit;
    Limit = M;
    Sibling = NULL;
    if (i == 0){
        if (Parent->Children[1]->KeyNum < Limit)//如果i是第一个找它的右边节点 
            Sibling = Parent->Children[1];
    }
    else if (Parent->Children[i - 1]->KeyNum < Limit) //先找i的左边 
        Sibling = Parent->Children[i - 1];
    else if (i + 1 < Parent->KeyNum && Parent->Children[i + 1]->KeyNum < Limit)//找i的右边 
	{
        Sibling = Parent->Children[i + 1];
    }
    return Sibling;
}
/* 查找兄弟节点，其关键字数大于M/2 ;没有返回NULL*/
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
/* 当要对X插入Key的时候，i是X在Parent的位置，j是Key要插入的位置
   当要对Parent插入X节点的时候，i是要插入的位置，Key和j的值没有用
*/
static Position InsertElement(int isKey, Position Parent,Position X,KeyType Key,int i,int j,int tno,int bno,int ofset){
    
    int k;
    if (isKey){
        /* 插入key */
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
        /* 插入节点 */
        /* 对树叶节点进行连接 */
        if (X->Children[0] == NULL)//x是叶子节点 
		{
            if (i > 0)
                Parent->Children[i - 1]->Next = X;
            X->Next = Parent->Children[i];
        }
        k = Parent->KeyNum - 1;
        //移动父节点中值，插入x 
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
 
/*当要对X移除Key的时候，i是X在Parent的位置，j是Key要移除的位置
   当要对Parent移除X节点的时候，i是要移除的位置，Key和j的值没有用
*/ 
static Position RemoveElement(int isKey, Position Parent,Position X,int i,int j){
    
    int k,Limit;
    
    if (isKey){
        Limit = X->KeyNum;
        /* 删除key */
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
        /* 删除节点 */
        
        /* 修改树叶节点的链接 */
        if (X->Children[0] == NULL && i > 0)// 判断x是不是叶子节点 
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
 
/* Src和Dst是两个相邻的节点，i是Src在Parent中的位置；
 将Src的元素移动到Dst中 ,n是移动元素的个数*/
static Position MoveElement(Position Src,Position Dst,Position Parent,int i,int n){
    KeyType TmpKey;
    Position Child;
    int j,SrcInFront;
    int tno,bno,ofset;
    SrcInFront = 0;
    
    if (Src->Key[0] < Dst->Key[0])
        SrcInFront = 1;
    
    j = 0;
    /* 节点Src在Dst前面 */
    if (SrcInFront){
        if (Src->Children[0] != NULL)//Src不是叶子节点 
		{
            while (j < n) {
                Child = Src->Children[Src->KeyNum - 1];//child是src最后一个子节点 
                RemoveElement(0, Src, Child, Src->KeyNum - 1, Unavailable);//移除src的child 
                InsertElement(0, Dst, Child, Unavailable, 0, Unavailable,-1,-1,-1);//将child插入dst 
                j++;
            }
        }else{
            while (j < n) {//叶子节点改变赋值情况 
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
        /* 将树叶节点重新连接 */
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
 
static BPlusTree SplitNode(Position Parent,Position X,int i){//拆页将一个节点拆成两半 
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
    
    if (Parent != NULL)/* 如果是X不是根，那么插入新的节点返回原节点 */
        InsertElement(0, Parent, NewNode, Unavailable, i + 1, Unavailable,-1,-1,-1);
    else{
        /* 如果是X是根，那么创建新的根并返回 */
        Parent = MallocNewNode();
        InsertElement(0, Parent, X, Unavailable, 0, Unavailable,-1,-1,-1);
        InsertElement(0, Parent, NewNode, Unavailable, 1, Unavailable,-1,-1,-1);
        
        return Parent;
    }
    
    return X;
}
 
/* 合并节点,X少于M/2关键字，S有大于或等于M/2个关键字*/
static Position MergeNode(Position Parent, Position X,Position S,int i){
    int Limit;
    
    /* S的关键字数目大于M/2 */
    if (S->KeyNum > LIMIT_M_2){
        /* 从S中移动一个元素到X中 */
        MoveElement(S, X, Parent, i,1);
    }else{
        /* 将X全部元素移动到S中，并把X删除 */
        Limit = X->KeyNum;
        MoveElement(X,S, Parent, i,Limit);
        RemoveElement(0, Parent, X, i, Unavailable);
        
        free(X);
        X = NULL;
    }
    
    return Parent;
}
 
static BPlusTree RecursiveInsert(BPlusTree T,KeyType Key,int i,BPlusTree Parent,int tno,int bno,int ofset){//插入函数 
    int j,Limit;
    Position Sibling;
    
    /* 查找分支 */
    j = 0;
    while (j < T->KeyNum && Key >= T->Key[j]){
        /* 重复值不插入，找到key插入的位置 */
        j++;
    }
    if (j != 0 && T->Children[0] != NULL) j--;
    
    /* 树叶 */
    if (T->Children[0] == NULL)
        T = InsertElement(1, Parent, T, Key, i, j,tno,bno,ofset);
    /* 内部节点 */
    else
        T->Children[j] = RecursiveInsert(T->Children[j], Key, j, T,tno,bno,ofset);
    
    /* 调整节点 */
    
    Limit = M;
    
    if (T->KeyNum > Limit){
        /* 根 */
        if (Parent == NULL){
            /* 分裂节点 */
            T = SplitNode(Parent, T, i);
        }
        else{
            Sibling = FindSibling(Parent, i);
            if (Sibling != NULL){
                /* 将T的一个元素（Key或者Child）移动的Sibing中 */
                MoveElement(T, Sibling, Parent, i, 1);
            }else{
                /* 分裂节点 */
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
 
/* 插入 */
extern BPlusTree Insert(BPlusTree T,KeyType Key,int tno,int bno,int ofset){
    return RecursiveInsert(T, Key, 0, NULL,tno,bno,ofset);
}
 
static BPlusTree RecursiveRemove(BPlusTree T,KeyType Key,int i,BPlusTree Parent){
    
    int j,NeedAdjust;
    Position Sibling,Tmp;
    
    Sibling = NULL;
    
    /* 查找分支 */
    j = 0;
    while (j < T->KeyNum && Key >= T->Key[j]){
        if (Key == T->Key[j])
            break;
        j++;
    }
    
    if (T->Children[0] == NULL){
        /* 没找到 */
        if (Key != T->Key[j] || j == T->KeyNum)
            return T;
    }else
        if (j == T->KeyNum || Key < T->Key[j]) j--;
    
    
    
    /* 树叶 */
    if (T->Children[0] == NULL){
        T = RemoveElement(1, Parent, T, i, j);
    }else{
        T->Children[j] = RecursiveRemove(T->Children[j], Key, j, T);
    }
    
    NeedAdjust = 0;
    /* 树的根或者是一片树叶，或者其儿子数在2到M之间 */
    if (Parent == NULL && T->Children[0] != NULL && T->KeyNum < 2)
        NeedAdjust = 1;
    /* 除根外，所有非树叶节点的儿子数在[M/2]到M之间。(符号[]表示向上取整) */
    else if (Parent != NULL && T->Children[0] != NULL && T->KeyNum < LIMIT_M_2)
        NeedAdjust = 1;
    /* （非根）树叶中关键字的个数也在[M/2]和M之间 */
    else if (Parent != NULL && T->Children[0] == NULL && T->KeyNum < LIMIT_M_2)
        NeedAdjust = 1;
    
    /* 调整节点 */
    if (NeedAdjust){
        /* 根 */
        if (Parent == NULL){
            if(T->Children[0] != NULL && T->KeyNum < 2){
                Tmp = T;
                T = T->Children[0];
                free(Tmp);
                return T;
            }
            
        }else{
            /* 查找兄弟节点，其关键字数目大于M/2 */
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
 
/* 删除 */
extern BPlusTree Remove(BPlusTree T,KeyType Key){
    return RecursiveRemove(T, Key, 0, NULL);
}
 
/* 销毁 */
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
 
/* 遍历 */
extern void Travel(BPlusTree T){
    RecursiveTravel(T, 0);
    printf("\n");
}
 
/* 遍历树叶节点的数据 */
extern void TravelData(BPlusTree T){
    Position Tmp;
    int i;
    if (T == NULL)
        return ;
    printf("All Data:");
    Tmp = T;
    while (Tmp->Children[0] != NULL)
        Tmp = Tmp->Children[0];
    /* 第一片树叶 */
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
		//printf("打开文件\n");
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
        //printf("保存成功\n");
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
	f = fopen(s, "rb+");//以rb+方式打开表二进制文件（需调整）
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
void SelectRecord(BPlusTree &BT,PageTable *&PT,BufferQue* &Buf,int ofset,char attrname[Max_Attribute_Num][20],int sum)//在TNo表查找一条记录
{				//CharBPlusTree T,PageTable *&PT,BufferQue* &Buf,int ofset,char attrname[Max_Attribute_Num][20],int sum
	int Tno=BT->tableno[ofset];
	int Bno=BT->blockno[ofset];
	int	offset=BT->offset[ofset];
	//检查块是否在缓冲区
	PageNode *p1 = selectBuf_Block(Buf,Bno,Tno);
	if(p1){//p1非空，即返回在缓冲区中可的块Bno 
	//	printf("表%s块%d在缓冲区页中\n",TableList[TNo],Bno);
		Block *c=p1->block;
		print_attrint(Tno,c->Re[offset],attrname,sum);
		/////////////////////////////////////////// 
	}
	else{
		Page *p2 = selectPT_Block(PT,Bno);
		if(p2){//p2非空，块存在但未在缓冲区 

			Block *b=FIFO(PT,Buf,p2); 
			print_attrint(Tno,b->Re[offset],attrname,sum);
		}
		else
			printf("表%d中找不到块%d\n",Tno,Bno);
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
