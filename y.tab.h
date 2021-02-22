/* A Bison parser, made by GNU Bison 3.7.5.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    T_PROGRAM = 258,               /* T_PROGRAM  */
    T_IDENTIFIER = 259,            /* T_IDENTIFIER  */
    T_USES = 260,                  /* T_USES  */
    T_TYPE = 261,                  /* T_TYPE  */
    T_VAR = 262,                   /* T_VAR  */
    T_BEGIN = 263,                 /* T_BEGIN  */
    T_END = 264,                   /* T_END  */
    T_FUNCTION = 265,              /* T_FUNCTION  */
    T_PROCEDURE = 266,             /* T_PROCEDURE  */
    T_ARRAY = 267,                 /* T_ARRAY  */
    T_OF = 268,                    /* T_OF  */
    T_CONST = 269,                 /* T_CONST  */
    T_IF = 270,                    /* T_IF  */
    T_THEN = 271,                  /* T_THEN  */
    T_ELSE = 272,                  /* T_ELSE  */
    T_FOR = 273,                   /* T_FOR  */
    T_TO = 274,                    /* T_TO  */
    T_DOWNTO = 275,                /* T_DOWNTO  */
    T_DO = 276,                    /* T_DO  */
    T_WRITE = 277,                 /* T_WRITE  */
    T_WRITELN = 278,               /* T_WRITELN  */
    T_INDEXTYPE = 279,             /* T_INDEXTYPE  */
    T_SINGLEEQ = 280,              /* T_SINGLEEQ  */
    T_INTVAL = 281,                /* T_INTVAL  */
    T_BOOLVAL = 282,               /* T_BOOLVAL  */
    T_FLOATVAL = 283,              /* T_FLOATVAL  */
    T_STRINGVAL = 284,             /* T_STRINGVAL  */
    T_DATATYPE = 285,              /* T_DATATYPE  */
    T_ASOP = 286,                  /* T_ASOP  */
    T_AS_PE = 287,                 /* T_AS_PE  */
    T_AS_SE = 288,                 /* T_AS_SE  */
    T_AS_MULE = 289,               /* T_AS_MULE  */
    T_AS_DIVE = 290,               /* T_AS_DIVE  */
    T_GE = 291,                    /* T_GE  */
    T_LE = 292,                    /* T_LE  */
    T_NE = 293,                    /* T_NE  */
    T_BOOL_AND = 294,              /* T_BOOL_AND  */
    T_BOOL_OR = 295,               /* T_BOOL_OR  */
    T_BOOL_NOT = 296,              /* T_BOOL_NOT  */
    T_BIT_LEFT_SHIFT = 297,        /* T_BIT_LEFT_SHIFT  */
    T_BIT_RIGHT_SHIFT = 298        /* T_BIT_RIGHT_SHIFT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define T_PROGRAM 258
#define T_IDENTIFIER 259
#define T_USES 260
#define T_TYPE 261
#define T_VAR 262
#define T_BEGIN 263
#define T_END 264
#define T_FUNCTION 265
#define T_PROCEDURE 266
#define T_ARRAY 267
#define T_OF 268
#define T_CONST 269
#define T_IF 270
#define T_THEN 271
#define T_ELSE 272
#define T_FOR 273
#define T_TO 274
#define T_DOWNTO 275
#define T_DO 276
#define T_WRITE 277
#define T_WRITELN 278
#define T_INDEXTYPE 279
#define T_SINGLEEQ 280
#define T_INTVAL 281
#define T_BOOLVAL 282
#define T_FLOATVAL 283
#define T_STRINGVAL 284
#define T_DATATYPE 285
#define T_ASOP 286
#define T_AS_PE 287
#define T_AS_SE 288
#define T_AS_MULE 289
#define T_AS_DIVE 290
#define T_GE 291
#define T_LE 292
#define T_NE 293
#define T_BOOL_AND 294
#define T_BOOL_OR 295
#define T_BOOL_NOT 296
#define T_BIT_LEFT_SHIFT 297
#define T_BIT_RIGHT_SHIFT 298

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 152 "pascal.y"

	struct parse_node
	{
		
		char *str;
		char *type;
		int intval;
		float floatval;
		// struct ast_node * ast;
	}s;

#line 165 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
