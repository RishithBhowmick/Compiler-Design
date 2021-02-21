/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_PROGRAM = 258,
    T_IDENTIFIER = 259,
    T_USES = 260,
    T_TYPE = 261,
    T_VAR = 262,
    T_BEGIN = 263,
    T_END = 264,
    T_FUNCTION = 265,
    T_PROCEDURE = 266,
    T_AND = 267,
    T_ARRAY = 268,
    T_OF = 269,
    T_CONST = 270,
    T_IF = 271,
    T_THEN = 272,
    T_ELSE = 273,
    T_FOR = 274,
    T_TO = 275,
    T_DOWNTO = 276,
    T_DO = 277,
    T_WRITE = 278,
    T_WRITELN = 279,
    T_INDEXTYPE = 280,
    T_SINGLEEQ = 281,
    T_INTVAL = 282,
    T_BOOLVAL = 283,
    T_FLOATVAL = 284,
    T_STRINGVAL = 285,
    T_DATATYPE = 286,
    T_ASOP = 287,
    T_AS_PE = 288,
    T_AS_SE = 289,
    T_AS_MULE = 290,
    T_AS_DIVE = 291,
    T_GE = 292,
    T_LE = 293,
    T_NE = 294,
    T_BOOL_AND = 295,
    T_BOOL_OR = 296,
    T_BOOL_NOT = 297,
    T_BIT_LEFT_SHIFT = 298,
    T_BIT_RIGHT_SHIFT = 299
  };
#endif
/* Tokens.  */
#define T_PROGRAM 258
#define T_IDENTIFIER 259
#define T_USES 260
#define T_TYPE 261
#define T_VAR 262
#define T_BEGIN 263
#define T_END 264
#define T_FUNCTION 265
#define T_PROCEDURE 266
#define T_AND 267
#define T_ARRAY 268
#define T_OF 269
#define T_CONST 270
#define T_IF 271
#define T_THEN 272
#define T_ELSE 273
#define T_FOR 274
#define T_TO 275
#define T_DOWNTO 276
#define T_DO 277
#define T_WRITE 278
#define T_WRITELN 279
#define T_INDEXTYPE 280
#define T_SINGLEEQ 281
#define T_INTVAL 282
#define T_BOOLVAL 283
#define T_FLOATVAL 284
#define T_STRINGVAL 285
#define T_DATATYPE 286
#define T_ASOP 287
#define T_AS_PE 288
#define T_AS_SE 289
#define T_AS_MULE 290
#define T_AS_DIVE 291
#define T_GE 292
#define T_LE 293
#define T_NE 294
#define T_BOOL_AND 295
#define T_BOOL_OR 296
#define T_BOOL_NOT 297
#define T_BIT_LEFT_SHIFT 298
#define T_BIT_RIGHT_SHIFT 299

/* Value type.  */


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
