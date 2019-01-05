
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "exp4.y"

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
char TABLE_LIST[MAX_OF_TABLE][20];

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
int item_No;
int type_No;



/* Line 189 of yacc.c  */
#line 133 "exp4.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NAME = 258,
     STRING = 259,
     NUMBER = 260,
     FLOATNUMBER = 261,
     CHAR = 262,
     VARCHAR = 263,
     LONG = 264,
     FLOAT = 265,
     DOUBLE = 266,
     COMPARISON = 267,
     PATH = 268,
     AND = 269,
     SELECT = 270,
     FROM = 271,
     WHERE = 272,
     ORDER = 273,
     BY = 274,
     ASC = 275,
     DESC = 276,
     ALL = 277,
     UNIQUE = 278,
     DISTINCT = 279,
     CREATE = 280,
     TABLE = 281,
     DROP = 282,
     LOAD = 283,
     INSERT = 284,
     INTO = 285,
     VALUES = 286,
     DELETE = 287,
     CHARACTER = 288,
     INTEGER = 289,
     DATE = 290,
     SHOW = 291,
     TABLES = 292,
     EXIT = 293,
     DATA = 294,
     HASH = 295,
     BPT = 296,
     HELP = 297,
     ON = 298,
     PRIMARY = 299,
     KEY = 300,
     UPDATE = 301,
     SET = 302,
     DESCRIBE = 303,
     ADMIN = 304
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 59 "exp4.y"

	int intval;
	char *strval;



/* Line 214 of yacc.c  */
#line 225 "exp4.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 237 "exp4.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  27
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   101

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  59
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  28
/* YYNRULES -- Number of rules.  */
#define YYNRULES  61
/* YYNRULES -- Number of states.  */
#define YYNSTATES  116

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   304

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      55,    56,    17,    15,    57,    16,     2,    18,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    54,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    58,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    11,    13,    15,    17,
      19,    21,    26,    32,    34,    42,    44,    46,    50,    53,
      58,    63,    65,    67,    69,    71,    73,    82,    94,    96,
      98,   102,   106,   108,   112,   118,   126,   129,   132,   133,
     135,   137,   139,   140,   144,   149,   154,   156,   160,   162,
     166,   167,   170,   172,   176,   180,   184,   188,   190,   193,
     195,   197
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      60,     0,    -1,    61,    -1,    42,    -1,    54,    -1,    65,
      -1,    62,    -1,    63,    -1,    74,    -1,    70,    -1,    73,
      -1,    31,    30,    66,    54,    -1,    32,    30,    66,    64,
      54,    -1,    13,    -1,    29,    30,    66,    55,    67,    56,
      54,    -1,     3,    -1,    68,    -1,    67,    57,    68,    -1,
      84,    69,    -1,     7,    55,     5,    56,    -1,     8,    55,
       5,    56,    -1,    38,    -1,     9,    -1,    10,    -1,    11,
      -1,    39,    -1,    33,    34,    66,    35,    55,    71,    56,
      54,    -1,    33,    34,    66,    55,    72,    56,    35,    55,
      71,    56,    54,    -1,     5,    -1,    83,    -1,    71,    58,
       5,    -1,    71,    58,    83,    -1,    85,    -1,    72,    57,
      85,    -1,    36,    20,    66,    80,    54,    -1,    19,    75,
      20,    78,    80,    77,    54,    -1,    76,    79,    -1,    76,
      17,    -1,    -1,    26,    -1,    28,    -1,    27,    -1,    -1,
      22,    23,    84,    -1,    22,    23,    84,    24,    -1,    22,
      23,    84,    25,    -1,    66,    -1,    78,    57,    66,    -1,
      84,    -1,    79,    57,    84,    -1,    -1,    21,    81,    -1,
      82,    -1,    81,    14,    82,    -1,    84,    86,    84,    -1,
      84,    86,     5,    -1,    84,    86,     4,    -1,     3,    -1,
      83,     3,    -1,     3,    -1,     3,    -1,    12,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   107,   107,   112,   118,   119,   120,   121,   122,   123,
     124,   129,   154,   170,   179,   202,   209,   214,   221,   229,
     236,   243,   250,   257,   264,   271,   281,   295,   317,   324,
     329,   336,   343,   348,   356,   380,   497,   498,   507,   510,
     514,   518,   523,   525,   529,   533,   539,   545,   552,   557,
     565,   568,   574,   578,   585,   596,   611,   625,   629,   637,
     643,   650
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NAME", "STRING", "NUMBER",
  "FLOATNUMBER", "CHAR", "VARCHAR", "LONG", "FLOAT", "DOUBLE",
  "COMPARISON", "PATH", "AND", "'+'", "'-'", "'*'", "'/'", "SELECT",
  "FROM", "WHERE", "ORDER", "BY", "ASC", "DESC", "ALL", "UNIQUE",
  "DISTINCT", "CREATE", "TABLE", "DROP", "LOAD", "INSERT", "INTO",
  "VALUES", "DELETE", "CHARACTER", "INTEGER", "DATE", "SHOW", "TABLES",
  "EXIT", "DATA", "HASH", "BPT", "HELP", "ON", "PRIMARY", "KEY", "UPDATE",
  "SET", "DESCRIBE", "ADMIN", "';'", "'('", "')'", "','", "'|'", "$accept",
  "sql_list", "sql", "drop_table", "load_table", "path", "create_table",
  "table_name", "attribute_def_list", "attribute_def_item", "data_type",
  "insert_stat", "value_list", "field_list", "delete_stat",
  "select_from_table", "select_clause", "unique", "order_clause",
  "fromlist", "sellist", "where_clause", "condition", "expr", "sentence",
  "attribute_name", "field_name", "cmp", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,    43,    45,    42,    47,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   298,   299,   300,
     301,   302,   303,   304,    59,    40,    41,    44,   124
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    59,    60,    60,    61,    61,    61,    61,    61,    61,
      61,    62,    63,    64,    65,    66,    67,    67,    68,    69,
      69,    69,    69,    69,    69,    69,    70,    70,    71,    71,
      71,    71,    72,    72,    73,    74,    75,    75,    76,    76,
      76,    76,    77,    77,    77,    77,    78,    78,    79,    79,
      80,    80,    81,    81,    82,    82,    82,    83,    83,    84,
      85,    86
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     5,     1,     7,     1,     1,     3,     2,     4,
       4,     1,     1,     1,     1,     1,     8,    11,     1,     1,
       3,     3,     1,     3,     5,     7,     2,     2,     0,     1,
       1,     1,     0,     3,     4,     4,     1,     3,     1,     3,
       0,     2,     1,     3,     3,     3,     3,     1,     2,     1,
       1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    38,     0,     0,     0,     0,     0,     3,     4,     0,
       2,     6,     7,     5,     9,    10,     8,    39,    41,    40,
       0,     0,     0,     0,     0,     0,     0,     1,     0,    59,
      37,    36,    48,    15,     0,     0,     0,     0,    50,    46,
      50,     0,     0,    11,    13,     0,     0,     0,     0,     0,
       0,    42,    49,     0,    16,     0,    12,     0,    60,     0,
      32,    51,    52,     0,    34,    47,     0,     0,     0,     0,
       0,     0,    22,    23,    24,    21,    25,    18,    57,    28,
       0,    29,     0,     0,     0,    61,     0,     0,    35,    14,
      17,     0,     0,     0,     0,    58,     0,    33,    53,    56,
      55,    54,    43,     0,     0,    26,    30,    31,     0,    44,
      45,    19,    20,     0,     0,    27
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     9,    10,    11,    12,    45,    13,    34,    53,    54,
      77,    14,    80,    59,    15,    16,    20,    21,    67,    40,
      31,    49,    61,    62,    81,    55,    60,    86
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -33
static const yytype_int8 yypact[] =
{
     -14,    -2,    12,    15,    23,   -13,    16,   -33,   -33,    54,
     -33,   -33,   -33,   -33,   -33,   -33,   -33,   -33,   -33,   -33,
      35,    -1,    55,    55,    55,    55,    55,   -33,    55,   -33,
     -33,     2,   -33,   -33,     5,     3,    48,   -32,    41,   -33,
     -20,    61,    61,   -33,   -33,    13,    14,    65,    61,    17,
      55,    50,   -33,    -9,   -33,     0,   -33,     1,   -33,    -7,
     -33,    56,   -33,    62,   -33,   -33,    52,    19,    22,    61,
      24,    25,   -33,   -33,   -33,   -33,   -33,   -33,   -33,   -33,
     -23,    74,    43,    65,    61,   -33,     9,    61,   -33,   -33,
     -33,    76,    77,    29,    38,   -33,    30,   -33,   -33,   -33,
     -33,   -33,    27,    28,    31,   -33,   -33,    74,     1,   -33,
     -33,   -33,   -33,   -12,    32,   -33
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -33,   -33,   -33,   -33,   -33,   -33,   -33,     6,   -33,    20,
     -33,   -33,   -18,   -33,   -33,   -33,   -33,   -33,   -33,   -33,
     -33,    51,   -33,     4,     7,   -21,    10,   -33
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      32,    48,    29,    46,    78,     1,    79,    70,    71,    72,
      73,    74,    29,    99,   100,     2,    30,     3,     4,     5,
      52,    25,     6,    47,    17,    18,    19,    63,     7,    35,
      36,    37,    38,    93,    39,    94,    26,    50,    75,    76,
       8,    78,    22,   106,   114,    23,    94,    68,    69,    82,
      83,   109,   110,    24,    27,    28,    65,    43,    33,    41,
      42,    44,    48,    63,    29,   101,   102,    56,    58,    57,
      84,    64,    66,    88,    85,    87,    89,    95,    96,    91,
      92,   103,   104,   105,   111,   108,   115,   112,    98,    90,
     113,    51,     0,    97,     0,     0,     0,     0,     0,     0,
       0,   107
};

static const yytype_int8 yycheck[] =
{
      21,    21,     3,    35,     3,    19,     5,     7,     8,     9,
      10,    11,     3,     4,     5,    29,    17,    31,    32,    33,
      41,    34,    36,    55,    26,    27,    28,    48,    42,    23,
      24,    25,    26,    56,    28,    58,    20,    57,    38,    39,
      54,     3,    30,     5,    56,    30,    58,    56,    57,    56,
      57,    24,    25,    30,     0,    20,    50,    54,     3,    57,
      55,    13,    21,    84,     3,    86,    87,    54,     3,    55,
      14,    54,    22,    54,    12,    23,    54,     3,    35,    55,
      55,     5,     5,    54,    56,    55,    54,    56,    84,    69,
     108,    40,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    19,    29,    31,    32,    33,    36,    42,    54,    60,
      61,    62,    63,    65,    70,    73,    74,    26,    27,    28,
      75,    76,    30,    30,    30,    34,    20,     0,    20,     3,
      17,    79,    84,     3,    66,    66,    66,    66,    66,    66,
      78,    57,    55,    54,    13,    64,    35,    55,    21,    80,
      57,    80,    84,    67,    68,    84,    54,    55,     3,    72,
      85,    81,    82,    84,    54,    66,    22,    77,    56,    57,
       7,     8,     9,    10,    11,    38,    39,    69,     3,     5,
      71,    83,    56,    57,    14,    12,    86,    23,    54,    54,
      68,    55,    55,    56,    58,     3,    35,    85,    82,     4,
       5,    84,    84,     5,     5,    54,     5,    83,    55,    24,
      25,    56,    56,    71,    56,    54
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 108 "exp4.y"
    {
			lineno = 1;
			return 0;
		;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 113 "exp4.y"
    {
			return -1;
		;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 130 "exp4.y"
    {
			//start = clock();
			//printf("drop table %s\n",$3);
			if(CHECK_TABLE((yyvsp[(3) - (4)].strval)) == 0){//表不存在
				printf("Table %s does not exist! Fail to drop table!\n",(yyvsp[(3) - (4)].strval));
				printf("Please check your sql order and input again!\n");
			}
			else{
				int Signal = DROP_TABLE((yyvsp[(3) - (4)].strval));
				if(Signal == 1)
					printf("success to drop table %s\n",(yyvsp[(3) - (4)].strval));
				else if(Signal == 2)
					printf("table %s does not exist\n",(yyvsp[(3) - (4)].strval));
				else
					printf("fail to drop table %s \n",(yyvsp[(3) - (4)].strval));
			}
			
			
			//end = clock();
			// yyPrintRunTime(start, end);
		;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 155 "exp4.y"
    {
			//start = clock();
			if(CHECK_TABLE((yyvsp[(3) - (5)].strval)) == 0){//表不存在
				printf("Table %s does not exist! Fail to load file into table!\n",(yyvsp[(3) - (5)].strval));
				printf("Please check your sql order and input again!\n");
			}
			else{
				printf("load table %s from file %s\n",(yyvsp[(3) - (5)].strval),(yyvsp[(4) - (5)].strval));
				LOAD_TABLE((yyvsp[(3) - (5)].strval),(yyvsp[(4) - (5)].strval));
			}
			//end = clock();
			// yyPrintRunTime(start, end);
		;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 171 "exp4.y"
    {
			(yyval.strval) = (yyvsp[(1) - (1)].strval);
			//printf("file: %s\n",$1);
		;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 180 "exp4.y"
    {
			//start = clock();
			//printf("grammar tree: create_table <- CREATE TABLE table_name '(' attribute_def_list%d')' ';'\n",ANo-1);
			if(CHECK_TABLE((yyvsp[(3) - (7)].strval)) == 1){//表存在
				printf("Table %s exist! Fail to create table!\n",(yyvsp[(3) - (7)].strval));
				printf("Please check your sql order and input again!\n");
			}
			else{
				int Signal = CREATE_TABLE((yyvsp[(3) - (7)].strval), ANo, R);
				if(Signal == 1)
					printf("success to create table %s \n",(yyvsp[(3) - (7)].strval));
				else if(Signal == 2)
					printf("table %s has already existed\n",(yyvsp[(3) - (7)].strval));
				else
					printf("fail to create table %s \n",(yyvsp[(3) - (7)].strval));
			}
			//printf("%d\n",R->Head->attrubuteNum);
			//end = clock();
			// yyPrintRunTime(start, end);
		;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 203 "exp4.y"
    {
			//printf("grammar tree: table_name <- NAME\n");
			(yyval.strval) = (yyvsp[(1) - (1)].strval);
		;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 210 "exp4.y"
    {
			//printf("grammar tree: attribute_def_list%d<- attribute_def_item%d\n",ANo,item_No-1);
			ANo ++;
		;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 215 "exp4.y"
    {
			//printf("grammar tree: attribute_def_list%d<- attribute_def_list%d',' attribute_def_item%d\n",ANo,ANo-1,item_No-1);
			ANo ++;
		;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 222 "exp4.y"
    {
			//printf("grammar tree: attribute_def_item%d<- %s data_type%d\n",item_No,$1,type_No-1);
			strcpy(R->attribute[ANo]->name, (yyvsp[(1) - (2)].strval));
			item_No ++
		;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 230 "exp4.y"
    {
			//printf("grammar tree: data_type%d<- CHAR '(' %d ')'\n",type_No,$3);
			R->attribute[ANo]->type = 1;
			R->attribute[ANo]->AttributeLength = (yyvsp[(3) - (4)].intval);
			type_No ++;
		;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 237 "exp4.y"
    {
			//printf("grammar tree: data_type%d<- VARCHAR '(' %d ')'\n",type_No,$3);
			R->attribute[ANo]->type = 2;
			R->attribute[ANo]->AttributeLength = (yyvsp[(3) - (4)].intval);
			type_No ++;
		;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 244 "exp4.y"
    {
			//printf("grammar tree: data_type%d<- INTEGER\n",type_No);
			R->attribute[ANo]->type = 3;
			R->attribute[ANo]->AttributeLength = 4;
			type_No ++;
		;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 251 "exp4.y"
    {
			//printf("grammar tree: data_type%d<- LONG\n",type_No);
			R->attribute[ANo]->type = 4;
			R->attribute[ANo]->AttributeLength = sizeof(long);
			type_No ++;
		;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 258 "exp4.y"
    {
			//printf("grammar tree: data_type%d<- FLOAT\n",type_No);
			R->attribute[ANo]->type = 5;
			R->attribute[ANo]->AttributeLength = sizeof(float);
			type_No ++;
		;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 265 "exp4.y"
    {
			//printf("grammar tree: data_type%d<- DOUBLE\n",type_No);
			R->attribute[ANo]->type = 6;
			R->attribute[ANo]->AttributeLength = sizeof(double);
			type_No ++;
		;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 272 "exp4.y"
    {
			//printf("grammar tree: data_type%d<- DATE\n",type_No);
			R->attribute[ANo]->type = 7;
			R->attribute[ANo]->AttributeLength = 8;
			type_No ++;
		;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 282 "exp4.y"
    {
			//start = clock();
			if(CHECK_TABLE((yyvsp[(3) - (8)].strval)) == 0){//表不存在
				printf("Table %s does not exist! Fail to insert record into table!\n",(yyvsp[(3) - (8)].strval));
				printf("Please check your sql order and input again!\n");
			}
			else{
				printf("%d\n",valcnt);
				INSERT_RECORD((yyvsp[(3) - (8)].strval),FIELD_LIST,fiecnt,VALUE_LIST,valcnt);
			}
			//end = clock();
			// yyPrintRunTime(start, end);
		;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 296 "exp4.y"
    {
			//start = clock();
			if(CHECK_TABLE((yyvsp[(3) - (11)].strval)) == 0){//表不存在
                printf("Table %s does not exist! Fail to insert record into table!\n",(yyvsp[(3) - (11)].strval));
				printf("Please check your sql order and input again!\n");
			}
			else{
				if(fiecnt == valcnt){
					printf("insert order valid!\n");
					INSERT_RECORD((yyvsp[(3) - (11)].strval),FIELD_LIST,fiecnt,VALUE_LIST,valcnt);
				}
				else{
					printf("insert order invalid!\n");
				}
			}
			
			//end = clock();
			// yyPrintRunTime(start, end);
		;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 318 "exp4.y"
    {
			char s[20];
			itoa((yyvsp[(1) - (1)].intval), s, 10);
			strcpy(VALUE_LIST[valcnt], s);
			valcnt ++;
		;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 325 "exp4.y"
    {
			strcpy(VALUE_LIST[valcnt], (yyvsp[(1) - (1)].strval));
			valcnt ++;
		;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 330 "exp4.y"
    {
			char s[20];
			itoa((yyvsp[(3) - (3)].intval), s, 10);
			strcpy(VALUE_LIST[valcnt], s);
			valcnt ++;
		;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 337 "exp4.y"
    {
			strcpy(VALUE_LIST[valcnt], (yyvsp[(3) - (3)].strval));
			valcnt ++;
		;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 344 "exp4.y"
    {
			strcpy(FIELD_LIST[fiecnt], (yyvsp[(1) - (1)].strval));
			fiecnt ++;
		;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 349 "exp4.y"
    {
			strcpy(FIELD_LIST[fiecnt], (yyvsp[(3) - (3)].strval));
			fiecnt ++;
		;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 357 "exp4.y"
    {
			//start = clock();
			if(CHECK_TABLE((yyvsp[(3) - (5)].strval)) == 0){//表不存在
                printf("Table %s does not exist! Fail to delete record from table!\n",(yyvsp[(3) - (5)].strval));
				printf("Please check your sql order and input again!\n");
			}
			else{
				DELETE_SIGN = 1;
				if(is_where == 0){
					printf("if you want to delete some records,your condition can not be null!\n");
				}
				else{
					printf("start delete!\n");
					select_attr((yyvsp[(3) - (5)].strval),ATTRIBUTE_LIST,selcnt,cond[0].attribute1,cond[0].operator0,cond[0].attribute2);
				}
			}
			
			//end = clock();
			// yyPrintRunTime(start, end);
		;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 381 "exp4.y"
    {
			//before = clock();
			//select_lqp(selcnt, selattrs, tbcnt, rel, condcnt, cond, orderby, orderattr, is_uni, is_ares);
			printf("number of table %d\n",tbcnt);
			if(tbcnt == 1)//单表选择
			{
				if(CHECK_TABLE((yyvsp[(4) - (7)].strval)) == 0){//表不存在
					printf("Table %s does not exist! Fail to select record from table!\n",(yyvsp[(4) - (7)].strval));
					printf("Please check your sql order and input again!\n");
				}
				else//表存在
				{
					if(selcnt == 0){//选择列为*
						int TNo = Get_TNo((yyvsp[(4) - (7)].strval));
						selcnt = Recinfo[TNo]->Head->attrubuteNum;
						for(int i=0;i<selcnt;i++){
							strcpy(ATTRIBUTE_LIST[i],Recinfo[TNo]->attribute[i]->name);
						}
					}

					if(condcnt == 0){//无条件投影
						//for(int i=0;i<selcnt;i++)
							//printf("%s ",ATTRIBUTE_LIST[i]);
						print_attr((yyvsp[(4) - (7)].strval),ATTRIBUTE_LIST,selcnt);
					}
					else//选择
						select_attr((yyvsp[(4) - (7)].strval),ATTRIBUTE_LIST,selcnt,cond[condcnt-1].attribute1,cond[condcnt-1].operator0,cond[condcnt-1].attribute2);
				}
				
			}
			else if(tbcnt == 2)//多表连接
			{
				if(CHECK_TABLE(TABLE_LIST[0]) == 0){//表1不存在
					printf("Table %s does not exist! Fail to select record from table!\n",TABLE_LIST[0]);
					printf("Please check your sql order and input again!\n");
				}
				else if(CHECK_TABLE(TABLE_LIST[1]) == 0){//表2不存在
					printf("Table %s does not exist! Fail to select record from table!\n",TABLE_LIST[1]);
					printf("Please check your sql order and input again!\n");
				}
				else{
					int TNo0,TNo1;
					TNo0 = Get_TNo(TABLE_LIST[0]);
					TNo1 = Get_TNo(TABLE_LIST[1]);
					if(selcnt == 0){//选择列为*
							//printf("!!!!!!\n");
							
							Record *join = recjoin(TABLE_LIST[0],TABLE_LIST[0]);
							selcnt = join->Head->attrubuteNum;
							for(int i=0;i<selcnt;i++){
								strcpy(ATTRIBUTE_LIST[i],join->attribute[i]->name);
							}
					}
					if(condcnt == 0){//无条件投影,卡式积
							//printf("卡式积\n");
						KaConnect(TNo0,TNo1,Buf,PTList[TNo0],PTList[TNo1]);
					}
					int OPTION = -1;
					if(cond[0].operator0[0] == '=')
						OPTION = 1;
					else if(cond[0].operator0[0] == '<'&&cond[0].operator0[1] == '>')
						OPTION = 0;
					if(OPTION != -1){
						
						int Length0 = TABLE_SIZE(TNo0);
						int Length1 = TABLE_SIZE(TNo1);
						printf("size of table %s:%d\n",TABLE_LIST[0],Length0);
						printf("size of table %s:%d\n",TABLE_LIST[1],Length1);
						if(Length0>Length1){
							printf("exchange the order of two table!\n");
							int tmp = TNo0;
							TNo0 = TNo1;
							TNo1 = tmp;
						}
						
						else{//选择
							//select_attr($4,ATTRIBUTE_LIST,selcnt,cond[condcnt-1].attribute1,cond[condcnt-1].operator0,cond[condcnt-1].attribute2);
							//void ConnectTable1(int TNo1,int TNo2,int ANo1,int ANo2,int Option,BufferQue* &Buf1,PageTable *PT1,PageTable *PT2)
							int ANo0 = FIND_ANO(TNo0,cond[0].attribute1);
							if(ANo0 != -1){
								int ANo1 = FIND_ANO(TNo1,cond[0].attribute2);
								if(ANo1 != -1){
									printf("start connect table!\n");
									ConnectTable1(TNo0,TNo1,ANo0,ANo1,OPTION,Buf,PTList[TNo0],PTList[TNo1]);
								}
								else{
									printf("you input extra table!\n");
								}
							}
							else{//交换选择属性名次序
								ANo0 = FIND_ANO(TNo1,cond[0].attribute1);
								int ANo1 = FIND_ANO(TNo0,cond[0].attribute2);
								if(ANo0 != -1&&ANo1 != -1){
									printf("exchange attributename and start connect table!\n");
									ConnectTable1(TNo0,TNo1,ANo1,ANo0,OPTION,Buf,PTList[TNo0],PTList[TNo1]);
								}
								else{
									printf("fail to exchange!\n");
								}
							}
							
						}
					}
					else{
						printf("Sorry,our DBMS does not support this operator when connencting table!\n");
					}	
				}
				

			}
			//after = clock();
			//print_runtime(before, after);
		;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 499 "exp4.y"
    {
			//selcnt=MAXCOND_SEL;
			selcnt=0;
			is_ares = 1;
		;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 507 "exp4.y"
    {
			is_uni = 0;
		;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 511 "exp4.y"
    {
			is_uni = 0;
		;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 515 "exp4.y"
    {
			is_uni = 1;
		;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 519 "exp4.y"
    {
			is_uni = 1;
		;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 526 "exp4.y"
    {
			orderby = 1;
		;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 530 "exp4.y"
    {
			orderby = 1;
		;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 534 "exp4.y"
    {
			orderby = 2;
		;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 540 "exp4.y"
    {
			strcpy(TABLE_LIST[tbcnt],(yyvsp[(1) - (1)].strval));
			tbcnt++;
			(yyval.strval) = (yyvsp[(1) - (1)].strval);
		;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 546 "exp4.y"
    {
			strcpy(TABLE_LIST[tbcnt],(yyvsp[(3) - (3)].strval));
			tbcnt++;
		;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 553 "exp4.y"
    {
			strcpy(ATTRIBUTE_LIST[selcnt],(yyvsp[(1) - (1)].strval));
			selcnt++;
		;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 558 "exp4.y"
    {
			strcpy(ATTRIBUTE_LIST[selcnt],(yyvsp[(3) - (3)].strval));
			selcnt++;
		;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 565 "exp4.y"
    {
			is_where = 0;
		;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 569 "exp4.y"
    {
			is_where = 1;
		;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 575 "exp4.y"
    {
			condcnt++;
		;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 579 "exp4.y"
    {
			
			condcnt++;
		;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 586 "exp4.y"
    {
			cond[condcnt].is_attribute = 1;
			op = (yyvsp[(2) - (3)].strval);   
			strcpy(cond[condcnt].attribute1 , (yyvsp[(1) - (3)].strval));
			strcpy(cond[condcnt].attribute2,(yyvsp[(3) - (3)].strval));
			strcpy(cond[condcnt].operator0 , (yyvsp[(2) - (3)].strval));
			/*printf("%s ",cond[condcnt].attribute1);
			printf("%s ",cond[condcnt].operator0);
			printf("%s\n",cond[condcnt].attribute2);*/
		;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 597 "exp4.y"
    {
			cond[condcnt].is_attribute = 0;
			op = (yyvsp[(2) - (3)].strval);
			//printf("%s\n",$1);
			strcpy(cond[condcnt].attribute1,(yyvsp[(1) - (3)].strval));
			char s[20];
			itoa((yyvsp[(3) - (3)].intval), s, 10);
			strcpy(cond[condcnt].attribute2,s);
			cond[condcnt].constantnumber = (yyvsp[(3) - (3)].intval);
			strcpy(cond[condcnt].operator0,(yyvsp[(2) - (3)].strval));
			/*printf("%s ",cond[condcnt].attribute1);
			printf("%s ",cond[condcnt].operator0);
			printf("%d\n",cond[condcnt].constantnumber);*/
		;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 612 "exp4.y"
    {
			cond[condcnt].is_attribute = 2;
			op = (yyvsp[(2) - (3)].strval);
			//printf("%s\n",$1);
			strcpy(cond[condcnt].attribute1,(yyvsp[(1) - (3)].strval));
			strcpy(cond[condcnt].attribute2,(yyvsp[(3) - (3)].strval));
			//strcpy(cond[condcnt].operator0,$2);
			//printf("%s ",cond[condcnt].attribute1);
			//printf("%s ",cond[condcnt].operator0);
			//printf("%s\n",cond[condcnt].attribute2);
		;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 626 "exp4.y"
    {
			(yyval.strval) = (yyvsp[(1) - (1)].strval);
		;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 630 "exp4.y"
    {
			char p[255];
			sprintf(p,"%s %s",(yyvsp[(1) - (2)].strval),(yyvsp[(2) - (2)].strval));
			strcpy((yyval.strval),p);
		;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 638 "exp4.y"
    {
			(yyval.strval) = (yyvsp[(1) - (1)].strval);
		;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 644 "exp4.y"
    {
			(yyval.strval) = (yyvsp[(1) - (1)].strval);
			//printf("%s\n",$$);
		;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 651 "exp4.y"
    {
			(yyval.strval) = (yyvsp[(1) - (1)].strval);
			//printf("%s\n",$$);
		;}
    break;



/* Line 1455 of yacc.c  */
#line 2269 "exp4.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 656 "exp4.y"

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
	DELETE_SIGN = 0;
	item_No = 0;
	type_No = 0;
	ANo = 0;
	return 0;
}

int main(){
	DBInit();
	printf("\nWelcome to our DBMS.\n");
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


