%{
#include <stdio.h>
#include <stdlib.h>
//#include <time.h>
#include "new.cpp"
#include <stdbool.h>
#include <string.h>
//#include "lex.yy.c"


// #define yyPrintRunTime(t1, t2) printf(" (%.3lf sec)\n\n", (double)((t2) - (t1)) / CLOCKS_PER_SEC)
#define MAXCOND_SEL 10
extern int lineno;
extern int yylex();
extern int yyerror(char *msg);
typedef struct ConDition{
	int is_attribute;//和属性比较为1，和数字比较为0,和字符串比较为2
	//char operator[4]; 
	char attribute1[20];
	char attribute2[20];
	char operator0[4]; 
	int constantnumber;
}ConDition;

/* info used for select statement */
static int is_ares;//是否选择全部属性 是为1，不是为0

static int tbcnt;//表数量

static int is_uni;

static int condcnt;//条件数量
ConDition cond[MAXCOND_SEL];

static int selcnt;//选择属性数量
char ATTRIBUTE_LIST[Max_Attribute_Num][20];

static int valcnt;//插入属性值数量
char VALUE_LIST[Max_Attribute_Num][255];

static int fiecnt;//插入属性名数量
char FIELD_LIST[Max_Attribute_Num][20];

static char *op;//操作符
/* used for order */
static int orderby;//orderby顺序
static int is_where;//是否有where
static int is_order;//是否有order
//clock_t start, end;
int ANo;
Record *R;


%}
%union {
	int intval;
	char *strval;
}

%start sql_list

%token <strval> NAME
%token <strval> STRING
%token <intval> NUMBER
%token <strval> CHAR
%token <strval> VARCHAR
%token <strval> LONG
%token <strval> FLOAT
%token <strval> DOUBLE
%token <strval> COMPARISON
%token <strval> PATH
%type <strval> table_name
%type <strval> attribute_name
%type <strval> field_name
%type <strval> path
%type <strval> cmp
%type <strval> fromlist
%type <strval> sentence
	/* operator */
%left AND
%left COMPARISON /* < <= > >= <> = */
%left '+' '-'
%left '*' '/'

	/* reserved keywords */
%token SELECT FROM WHERE ORDER BY ASC DESC
%token ALL UNIQUE DISTINCT
%token CREATE TABLE DROP
%token LOAD
%token INSERT INTO VALUES DELETE
%token CHARACTER INTEGER DATE
%token SHOW TABLES
%token EXIT
%token DATA HASH BPT HELP ON PRIMARY KEY
%token UPDATE SET DESCRIBE
%token ADMIN

%%

	/* start place */
sql_list:
		sql
		{
			lineno = 1;
			return 0;
		}
	|	EXIT
		{
			return -1;
		}
	;
sql:
		';'
	|	create_table
	|	drop_table
	|	load_table
	|   select_from_table
	|	insert_stat
	;

/* drop table */
drop_table:
		DROP TABLE table_name ';'
		{
			//start = clock();
			//printf("drop table %s\n",$3);
			
			int Signal = DROP_TABLE($3);
			if(Signal == 1)
				printf("success to drop table %s\n",$3);
			else if(Signal == 2)
				printf("table %s does not exist\n",$3);
			else
				printf("fail to drop table %s \n",$3);
			
			//end = clock();
			// yyPrintRunTime(start, end);
		}
	;
/* load table */
load_table:
		LOAD TABLE table_name path ';'
		{
			//start = clock();
			printf("load table %s from file %s\n",$3,$4);
			LOAD_TABLE($3,$4);
			//end = clock();
			// yyPrintRunTime(start, end);
		}
	;
path:
		PATH
		{
			$$ = $1;
			//printf("file: %s\n",$1);
		}
	;

/* create table */
create_table:
		CREATE TABLE table_name '(' attribute_def_list ')' ';'
		{
			//start = clock();
			int Signal = CREATE_TABLE($3, ANo, R);
			if(Signal == 1)
				printf("success to create table %s \n",$3);
			else if(Signal == 2)
				printf("table %s has already existed\n",$3);
			else
				printf("fail to create table %s \n",$3);
			ANo = 0;
			//printf("%d\n",R->Head->attrubuteNum);
			//end = clock();
			// yyPrintRunTime(start, end);
		}
	;
table_name:
		NAME
		{
			$$ = $1;
		}
	;
attribute_def_list:
		attribute_def_item
		{
			ANo ++;
		}
	|	attribute_def_list ',' attribute_def_item
		{
			ANo ++;
		}
	;
attribute_def_item:
		attribute_name data_type
		{
			strcpy(R->attribute[ANo]->name, $1);
		}
	;
data_type:
		CHAR '(' NUMBER ')'
		{
			//R = InputRecord(R,ANo,1);
			R->attribute[ANo]->type = 1;
			R->attribute[ANo]->AttributeLength = $3;
		}
	|	VARCHAR '(' NUMBER ')'
		{
			R->attribute[ANo]->type = 2;
			R->attribute[ANo]->AttributeLength = $3;
		}
	|	INTEGER
		{
			R->attribute[ANo]->type = 3;
			R->attribute[ANo]->AttributeLength = 4;
		}
	|	LONG
		{
			R->attribute[ANo]->type = 4;
			R->attribute[ANo]->AttributeLength = sizeof(long);
		}
	|	FLOAT
		{
			R->attribute[ANo]->type = 5;
			R->attribute[ANo]->AttributeLength = sizeof(float);
		}
	|	DOUBLE
		{
			R->attribute[ANo]->type = 6;
			R->attribute[ANo]->AttributeLength = sizeof(double);
		}			
	|	DATE
		{
			R->attribute[ANo]->type = 7;
			R->attribute[ANo]->AttributeLength = 8;
		}
	;
/* insert statements */
insert_stat:
		INSERT INTO table_name VALUES '(' value_list ')' ';'
		{
			//start = clock();
			printf("%d\n",valcnt);
			INSERT_RECORD($3,FIELD_LIST,fiecnt,VALUE_LIST,valcnt);
			//end = clock();
			// yyPrintRunTime(start, end);
		}
	|	INSERT INTO table_name '(' field_list ')' VALUES '(' value_list ')' ';'
		{
			//start = clock();
			if(fiecnt == valcnt){
				printf("insert order valid!\n");
				INSERT_RECORD($3,FIELD_LIST,fiecnt,VALUE_LIST,valcnt);
			}
			else{
				printf("insert order invalid!\n");
			}
			//end = clock();
			// yyPrintRunTime(start, end);
		}
	;
value_list:
		NUMBER
		{
			char s[20];
			itoa($1, s, 10);
			strcpy(VALUE_LIST[valcnt], s);
			valcnt ++;
		}
	|	sentence
		{
			strcpy(VALUE_LIST[valcnt], $1);
			valcnt ++;
		}
	|	value_list '|' NUMBER
		{
			char s[20];
			itoa($3, s, 10);
			strcpy(VALUE_LIST[valcnt], s);
			valcnt ++;
		}
	|	value_list '|' sentence
		{
			strcpy(VALUE_LIST[valcnt], $3);
			valcnt ++;
		}
	;
field_list:
		field_name
		{
			strcpy(FIELD_LIST[fiecnt], $1);
			fiecnt ++;
		}
	|	field_list ',' field_name
		{
			strcpy(FIELD_LIST[fiecnt], $3);
			fiecnt ++;
		}
	;
select_from_table:  
		SELECT select_clause FROM fromlist where_clause order_clause ';'
		{
			//before = clock();
			//select_lqp(selcnt, selattrs, tbcnt, rel, condcnt, cond, orderby, orderattr, is_uni, is_ares);
			if(selcnt == 0){
				int TNo = Get_TNo($4);
				selcnt = Recinfo[TNo]->Head->attrubuteNum;
				for(int i=0;i<selcnt;i++){
					strcpy(ATTRIBUTE_LIST[i],Recinfo[TNo]->attribute[i]->name);
				}
			}
			if(condcnt == 0){
				for(int i=0;i<selcnt;i++)
					printf("%s ",ATTRIBUTE_LIST[i]);
				print_attr($4,ATTRIBUTE_LIST,selcnt);//投影，无查找
			}
			else
				select_attr($4,ATTRIBUTE_LIST,selcnt,cond[condcnt-1].attribute1,cond[condcnt-1].operator0,cond[condcnt-1].attribute2);
			//printf("%s %s %s\n",cond[condcnt-1].attribute1,cond[condcnt-1].operator0,cond[condcnt-1].attribute2);
			printf("select %d from %d table which have %d condition where \n",selcnt,tbcnt,condcnt);
			//after = clock();
			//print_runtime(before, after);
		}
	;

select_clause:   //选择列 
		unique sellist
	|	unique '*'       
		{
			//selcnt=MAXCOND_SEL;
			selcnt=0;
			is_ares = 1;
		}
	;
unique:
		/* empty */
		{
			is_uni = 0;
		}
	|	ALL
		{
			is_uni = 0;
		}
	|	DISTINCT     //去重操作 
		{
			is_uni = 1;
		}
	|	UNIQUE
		{
			is_uni = 1;
		}
	;
order_clause:  //排序 
		/* empty */
	|	ORDER BY attribute_name
		{
			orderby = 1;
		}
	|	ORDER BY attribute_name ASC
		{
			orderby = 1;
		}
	|	ORDER BY attribute_name DESC
		{
			orderby = 2;
		}
	;
fromlist:    //from之后tables 
		table_name   //单个表 
		{
			tbcnt++;
			$$ = $1;
		}
	|	fromlist ',' table_name   //多个表 
		{
			tbcnt++;
		}
	;
sellist:  //select 属性列 
		attribute_name    //单个列显示 
		{
			strcpy(ATTRIBUTE_LIST[selcnt],$1);
			selcnt++;
		}
	|	sellist ',' attribute_name  //多个列显示 
		{
			strcpy(ATTRIBUTE_LIST[selcnt],$3);
			selcnt++;
		}
	;
where_clause:      //条件语句 
		/* empty */   //无条件查询 
		{
			is_where = 0;
		}
	|	WHERE condition    //有条件查询 
		{
			is_where = 1;
		}
	;
condition:  //条件定义 
		expr
		{
			condcnt++;
		}
	|	condition AND expr   //逻辑与 
		{
			
			condcnt++;
		}
	;
expr:   //条件表达式 
		attribute_name cmp attribute_name  //列之间比较 
		{
			cond[condcnt].is_attribute = 1;
			op = $2;   
			/*if (strcmp(op, "=") == 0) strcpy(cond[condcnt].operator , 'EQ');
			else if (strcmp(op, ">=") == 0) strcpy(cond[condcnt].operator , 'GE');
			else if (strcmp(op, "<=") == 0) strcpy(cond[condcnt].operator , 'LE');
			else if (strcmp(op, ">") == 0) strcpy(cond[condcnt].operator , 'GT');
			else if (strcmp(op, "<") == 0) strcpy(cond[condcnt].operator , 'LT');
			else if (strcmp(op, "<>") == 0) strcpy(cond[condcnt].operator , 'NE');*/
			strcpy(cond[condcnt].attribute1 , $1);
			strcpy(cond[condcnt].attribute2,$3);
			strcpy(cond[condcnt].operator0 , $2);
			printf("%s ",cond[condcnt].attribute1);
			printf("%s ",cond[condcnt].operator0);
			printf("%s\n",cond[condcnt].attribute2);
		}
	|	attribute_name cmp NUMBER  //列和数字比较 
		{
			cond[condcnt].is_attribute = 0;
			op = $2;
			/*if (strcmp(op, "=") == 0) strcpy(cond[condcnt].operator , 'EQ');
			else if (strcmp(op, ">=") == 0) strcpy(cond[condcnt].operator , 'GE');
			else if (strcmp(op, "<=") == 0) strcpy(cond[condcnt].operator , 'LE');
			else if (strcmp(op, ">") == 0) strcpy(cond[condcnt].operator , 'GT');
			else if (strcmp(op, "<") == 0) strcpy(cond[condcnt].operator , 'LT');
			else if (strcmp(op, "<>") == 0) strcpy(cond[condcnt].operator , 'NE');*/
			//printf("%s\n",$1);
			strcpy(cond[condcnt].attribute1,$1);
			char s[20];
			itoa($3, s, 10);
			strcpy(cond[condcnt].attribute2,s);
			cond[condcnt].constantnumber = $3;
			strcpy(cond[condcnt].operator0,$2);
			printf("%s ",cond[condcnt].attribute1);
			printf("%s ",cond[condcnt].operator0);
			printf("%d\n",cond[condcnt].constantnumber);
		}
	|	attribute_name cmp STRING  //列和字符串比较 
		{
			cond[condcnt].is_attribute = 2;
			op = $2;
			/*if (strcmp(op, "=") == 0) strcpy(cond[condcnt].operator , 'EQ');
			else if (strcmp(op, ">=") == 0) strcpy(cond[condcnt].operator , 'GE');
			else if (strcmp(op, "<=") == 0) strcpy(cond[condcnt].operator , 'LE');
			else if (strcmp(op, ">") == 0) strcpy(cond[condcnt].operator , 'GT');
			else if (strcmp(op, "<") == 0) strcpy(cond[condcnt].operator , 'LT');
			else if (strcmp(op, "<>") == 0) strcpy(cond[condcnt].operator , 'NE');*/
			//printf("%s\n",$1);
			strcpy(cond[condcnt].attribute1,$1);
			strcpy(cond[condcnt].attribute2,$3);
			//strcpy(cond[condcnt].operator0,$2);
			//printf("%s ",cond[condcnt].attribute1);
			//printf("%s ",cond[condcnt].operator0);
			//printf("%s\n",cond[condcnt].attribute2);
		}
	;
sentence:
		NAME 
		{
			$$ = $1;
		}
	|	sentence NAME
		{
			char p[255];
			sprintf(p,"%s %s",$1,$2);
			strcpy($$,p);
		}
	;
attribute_name:
		NAME
		{
			$$ = $1;
			//printf("%s\n",$$);
		}
	;
field_name:
		NAME
		{
			$$ = $1;
			//printf("%s\n",$$);
		}
	;
cmp:
		COMPARISON
		{
			$$ = $1;
			//printf("%s\n",$$);
		}
	;
%%
int parserInit()
{
	memset(cond, 0, sizeof(ConDition)*MAXCOND_SEL);
	orderby=0;
	tbcnt=0;
	is_where=0;
	is_ares=0;
	selcnt=0;
	condcnt=0;
	is_uni=0;
	valcnt=0;
	fiecnt=0;
	R = InitRecord();
	return 0;
}

int main(){
	DBInit();
	printf("\nWelcome to our DBMS. Type 'help;' for help info.\n");
	printf("SQL> ");
	parserInit();
	while(true){
		parserInit();
		if(yyparse()==-1){
			break;
		}
	}
	DBExit();
	printf("Goodbye ^_^\n\n");
	//getchar();
}

