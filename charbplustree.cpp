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

/*B+树的节点结构*/ 
struct CharBPlusNode{
    int KeyNum;//键值个数 
    char *Key[M + 1];//键值数组 
    int tableno[M + 1];
	int blockno[M + 1];
	int offset[M + 1]; 
    CharBPlusTree Children[M + 1];//子节点 
    CharBPlusTree Next;//兄弟节点 
};
 
static int Unavailable1 = INT_MIN;

/* 生成节点并初始化 */
static CharBPlusTree MallocNewCharNode(){
    CharBPlusTree NewNode;
    int i;
    NewNode = (CharBPlusNode*)malloc(sizeof(CharBPlusNode));
    if (NewNode == NULL)
        exit(EXIT_FAILURE);
    i = 0;
    while (i < M + 1){
    	NewNode->Key[i]=(char*)malloc(255*sizeof(char));//结点空间char[255] 
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
 
/* 初始化 */
extern CharBPlusTree InitializeCharTree(){
    CharBPlusTree T;    
    if (M < (3)){
        printf("M最小等于3！");
        exit(EXIT_FAILURE);
        
    }
    /* 根结点 */
    T = MallocNewCharNode();
    return T;
}
/*找到最左边的叶子节点*/ 
static CharPosition FindMostLeftChar(CharPosition P){
    CharPosition Tmp;
    Tmp = P;
    while (Tmp != NULL && Tmp->Children[0] != NULL) {
        Tmp = Tmp->Children[0];
    }
    return Tmp;
}
/*找到最右边的叶子节点*/ 
static CharPosition FindMostRightChar(CharPosition P){
    CharPosition Tmp;
    Tmp = P;
    while (Tmp != NULL && Tmp->Children[Tmp->KeyNum - 1] != NULL) {
        Tmp = Tmp->Children[Tmp->KeyNum - 1];
    }
    return Tmp;
}
/*寻找parent的children[i]的一个兄弟节点，其存储的关键字未满，否则返回NULL */
static CharPosition FindSiblingChar(CharPosition Parent,int i){
    CharPosition Sibling;
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
/* 当要对X插入Key的时候，i是X在Parent的位置，j是Key要插入的位置
   当要对Parent插入X节点的时候，i是要插入的位置，Key和j的值没有用
*/
static CharPosition InsertElementChar(int isKey, CharPosition Parent,CharPosition X,char *Key,int i,int j,int tno,int bno,int ofset){
    
    int k;
    if (isKey){
        /* 插入key */
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
 
/*当要对X移除Key的时候，i是X在Parent的位置，j是Key要移除的位置
   当要对Parent移除X节点的时候，i是要移除的位置，Key和j的值没有用
*/ 
static CharPosition RemoveElementChar(int isKey, CharPosition Parent,CharPosition X,int i,int j){
    
    int k,Limit;
    
    if (isKey){
        Limit = X->KeyNum;
        /* 删除key */
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
 
/* Src和Dst是两个相邻的节点，i是Src在Parent中的位置；
 将Src的元素移动到Dst中 ,n是移动元素的个数*/
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
    /* 节点Src在Dst前面 */
    if (SrcInFront){
        if (Src->Children[0] != NULL)//Src不是叶子节点 
		{
            while (j < n) {
                Child = Src->Children[Src->KeyNum - 1];//child是src最后一个子节点 
                RemoveElementChar(0, Src, Child, Src->KeyNum - 1, Unavailable1);//移除src的child 
                InsertElementChar(0, Dst, Child, "-111111111", 0, Unavailable1,-1,-1,-1);//将child插入dst 
                j++;
            }
        }else{
            while (j < n) {//叶子节点改变赋值情况 
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
        /* 将树叶节点重新连接 */
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
 
static CharBPlusTree SplitNodeChar(CharPosition Parent,CharPosition X,int i){//拆页将一个节点拆成两半 
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
    
    if (Parent != NULL)/* 如果是X不是根，那么插入新的节点返回原节点 */
        InsertElementChar(0, Parent, NewNode, "-111111111", i + 1, Unavailable1,-1,-1,-1);
    else{
        /* 如果是X是根，那么创建新的根并返回 */
        Parent = MallocNewCharNode();
        InsertElementChar(0, Parent, X, "-111111111", 0, Unavailable1,-1,-1,-1);
        InsertElementChar(0, Parent, NewNode, "-111111111", 1, Unavailable1,-1,-1,-1);
        
        return Parent;
    }
    
    return X;
}
 
/* 合并节点,X少于M/2关键字，S有大于或等于M/2个关键字*/
static CharPosition MergeNodeChar(CharPosition Parent, CharPosition X,CharPosition S,int i){
    int Limit;
    
    /* S的关键字数目大于M/2 */
    if (S->KeyNum > LIMIT_M_2){
        /* 从S中移动一个元素到X中 */
        MoveElementChar(S, X, Parent, i,1);
    }else{
        /* 将X全部元素移动到S中，并把X删除 */
        Limit = X->KeyNum;
        MoveElementChar(X,S, Parent, i,Limit);
        RemoveElementChar(0, Parent, X, i, Unavailable1);
        
        free(X);
        X = NULL;
    }
    
    return Parent;
}
 
static CharBPlusTree RecursiveInsertChar(CharBPlusTree T,char *Key,int i,CharBPlusTree Parent,int tno,int bno,int ofset){//插入函数 
    int j,Limit;
    CharPosition Sibling;
    
    /* 查找分支 */
    j = 0;
    while (j < T->KeyNum && strcmp(Key,T->Key[j])>=0){
        /* 重复值不插入，找到key插入的位置 */
        //if (strcmp(Key,T->Key[j])==0)
        //  return T;
        j++;
    }
    
    if (j != 0 && T->Children[0] != NULL) j--;
    //printf("%s %d\n",Key,j);
    /* 树叶 */
    if (T->Children[0] == NULL)
        T = InsertElementChar(1, Parent, T, Key, i, j,tno,bno,ofset);
    /* 内部节点 */
    else
        T->Children[j] = RecursiveInsertChar(T->Children[j], Key, j, T,tno,bno,ofset);
    
    /* 调整节点 */
    
    Limit = M;
    
    if (T->KeyNum > Limit){
        /* 根 */
        if (Parent == NULL){
            /* 分裂节点 */
            T = SplitNodeChar(Parent, T, i);
        }
        else{
            Sibling = FindSiblingChar(Parent, i);
            if (Sibling != NULL){
                /* 将T的一个元素（Key或者Child）移动的Sibing中 */
                MoveElementChar(T, Sibling, Parent, i, 1);
            }else{
                /* 分裂节点 */
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
 
/* 插入 */
extern CharBPlusTree InsertChar(CharBPlusTree T,char *Key,int tno,int bno,int ofset){
    return RecursiveInsertChar(T, Key, 0, NULL,tno,bno,ofset);
}
 
static CharBPlusTree RecursiveRemoveChar(CharBPlusTree T,char *Key,int i,CharBPlusTree Parent){
    
    int j,NeedAdjust;
    CharPosition Sibling,Tmp;
    
    Sibling = NULL;
    
    /* 查找分支 */
    j = 0;
    while (j < T->KeyNum && strcmp(Key,T->Key[j])!=-1){
        if (strcmp(Key,T->Key[j])==0)
            break;
        j++;
    }
    
    if (T->Children[0] == NULL){
        /* 没找到 */
        if (strcmp(Key,T->Key[j])!=0 || j == T->KeyNum)
            return T;
    }else
        if (j == T->KeyNum || strcmp(Key,T->Key[j])==-1) j--;
    
    
    
    /* 树叶 */
    if (T->Children[0] == NULL){
        T = RemoveElementChar(1, Parent, T, i, j);
    }else{
        T->Children[j] = RecursiveRemoveChar(T->Children[j], Key, j, T);
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
 
/* 删除 */
extern CharBPlusTree RemoveChar(CharBPlusTree T,char *Key){
    return RecursiveRemoveChar(T, Key, 0, NULL);
}
 
/* 销毁 */
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
 
/* 遍历 */
extern void TravelCharTree(CharBPlusTree T){
	//printf("dfasdfasdfwe342123412");
    RecursiveTravelCharTree(T, 0);
    printf("\n");
}
 
/* 遍历树叶节点的数据 */
extern void TravelCharTreeData(CharBPlusTree T){
    CharPosition Tmp;
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
void SelectCharRecord(CharBPlusTree T,PageTable *&PT,BufferQue* &Buf,int ofset,char attrname[Max_Attribute_Num][20],int sum)//在TNo表查找一条记录
{
	int Tno=T->tableno[ofset];
	int Bno=T->blockno[ofset];
	int	offset=T->offset[ofset];
	//检查块是否在缓冲区
	PageNode *p1 = selectBuf_Block(Buf,Bno,Tno);
	if(p1){//p1非空，即返回在缓冲区中可的块Bno 
		printf("表%s块%d在缓冲区页中\n",TableList[Tno],Bno);
		Block *c=p1->block;
		printf("%s\n",c->Re[offset]);
		print_attr2(Tno,c->Re[offset],attrname,sum);
		/////////////////////////////////////////// 
	}
	else{
		Page *p2 = selectPT_Block(PT,Bno);
		if(p2){//p2非空，块存在但未在缓冲区 
			Block *b=FIFO(PT,Buf,p2);
			printf("%s\n",b->Re[offset]);
			print_attr2(Tno,b->Re[offset],attrname,sum);
		}
		else
			printf("表%d中找不到块%d\n",Tno,Bno);
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
