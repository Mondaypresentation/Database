
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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

/* Line 1676 of yacc.c  */
#line 59 "exp4.y"

	int intval;
	char *strval;



/* Line 1676 of yacc.c  */
#line 108 "exp4.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


