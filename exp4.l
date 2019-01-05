%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exp4.tab.h"

int lineno = 1;
void yyerror(char *msg);
extern "C" int yywrap(void);

%}
%%

	/* reserved keywords */
CREATE	|
create		return CREATE;
DROP	|
drop		return DROP;
LOAD	|
load		return LOAD;
TABLE	|
table		return TABLE;

SELECT	|
select		return SELECT;
FROM	|
from		return FROM;
WHERE	|
where		return WHERE;
ORDER	|
order		return ORDER;
BY	|
by		return BY;
ASC | 
asc     return ASC;
DESC |
desc    return DESC;
UNIQUE	|
unique		return UNIQUE;
ALL	|
all		return ALL;
DISTINCT |
distinct	return DISTINCT;
INSERT	|
insert		return INSERT;
INTO	|
into		return INTO;
VALUES	|
values		return VALUES;
DELETE	|
delete		return DELETE;
UPDATE |
update  return UPDATE;
SET |
set return SET;
ADMIN |
admin return ADMIN;
AND	|
and		return AND;
INT(EGER)? |
int(eger)?	return INTEGER;
CHAR(ACTER)? |
char(acter)?	return CHAR;
long 	|
LONG 	return LONG;
FLOAT	|
float return FLOAT;
varchar |
VARCHAR return VARCHAR;

DOUBLE |
double return DOUBLE;
EXIT	|
exit	|
QUIT	|
quit		return EXIT;
DATE	|
date		return DATE;
SHOW	|
show		return SHOW;
TABLES	|
tables		return TABLES;
ON	|
on		return ON;
BPT	|
bpt		return BPT;
HASH	|
hash		return HASH;
HELP	|
help		return HELP;

DATA	|
data		return DATA;
	/* punctation */
"="	|
"<>"	|
"<"	|
">"	|
"<="	|
">="	{
		yylval.strval = strdup(yytext);
		return COMPARISON;
	}

[-+*/:(),|;]	{return yytext[0];}

	/* names */
[a-zA-Z][a-zA-Z0-9_]*	{
		yylval.strval = strdup(yytext);
		return NAME;
	}
[a-zA-Z]+[.][a-zA-Z]+	{
		yylval.strval = strdup(yytext);
		return PATH;
}
	/* numbers */
[1-9][0-9]*	{
		yylval.intval = atoi(yytext);
		return NUMBER;
	}
	/* floatnumber */
[1-9][0-9]*[.][0-9]+	{
		yylval.strval = strdup(yytext);
		return FLOATNUMBER;
	}
	
	/* strings */
'[^\n']*'	{
		yylval.strval = strdup(yytext);
		strcpy(yylval.strval, yytext + 1);
		*(yylval.strval + strlen(yytext) - 2) = '\0';
		return STRING;
	}

	/* whitespace */
\n	{
		if (lineno==1)printf("SQL> ");
		else printf("   > ");
		lineno++;
	}
[ \t\r]+	;

	/* anything else */
.	{
		yyerror("invalid character");
	}
%%

void yyerror(char *msg)
{
	printf("Wrong SQL : There is %s near '%s' (line %d).\n", msg, yytext, lineno);
}

int yywrap()
{
	return 1;
}