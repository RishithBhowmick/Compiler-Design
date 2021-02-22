/* A Bison parser, made by GNU Bison 3.7.5.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30705

/* Bison version string.  */
#define YYBISON_VERSION "3.7.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "pascal.y"

    #include<stdio.h>
    #include<stdlib.h>
    #include<time.h>
    #include "uthash/src/uthash.h"
    #include "sym_tab.h" 
    #define YYPARSE_PARAM scanner
    #define YYLEX_PARAM   scanner
    int yylex();
    int yyerror();
    int successful=1;

    extern FILE *yyin;
    extern FILE *yyout;
    double time_elapsed(struct timespec *start, struct timespec *end);
    
	char *var_name_stack[100];
	int var_name_stack_top = -1;

	char *assignment_name_stack[31];
	int assignment_name_stack_top = -1;
	
	char *curr_scope_level = "global";

	// struct variable_type_info var_type_information;
	struct symbol_table *SYMBOL_TABLE = NULL; /*Generic Symbol Table*/
	// We are using a hash table as the data structure for the symbol table
	//	Hash tables are efficient data structures for this purpose 
	//  because the lookup and insertion is constant time
	
	char *type_identifier_stack[10];
	int type_identifier_top = -1;

	struct type_table{
		char user_defined_name[31];
		char actual_type_name[31];
		UT_hash_handle hh;
	};
	
	struct const_table{
		char var_name[31];
		char* type;
		union data var_value;
		UT_hash_handle hh;
	};
	struct const_table* CONST_TABLE = NULL;
	struct type_table *TYPE_TABLE = NULL;

	int dump_stack_in_symbol_table(char *type, int line_no, int col_no) {
		for(int i = 0; i <= var_name_stack_top; i++) {
			struct symbol_table *s = NULL;
			char var_mang_name[31];
			strcpy(var_mang_name, var_name_stack[i]);
			strcat(var_mang_name, "$");
			strcat(var_mang_name, curr_scope_level);
			HASH_FIND_STR(SYMBOL_TABLE, var_mang_name, s);
			if(!s)
			{
				printf("Alert : Inserting Variable '%s' in to the Symbol Table.\n", var_mang_name);
				s = malloc(sizeof(struct symbol_table));
				strcat(s->var_name, var_mang_name);
				strcpy(s->type, type);
				//printf("Type : %d\n",strcmp(type,"integer"));
				s->scope_level = strdup(curr_scope_level);
				s->line_no = line_no;
				s->col_no = col_no;
				if(strcmp(type,"string")==0){
					strcpy(s->var_value.string_value, "");
				}
				else if(strcmp(type,"integer")==0){
					s->var_value.int_value = 0;
				}
				else if(strcmp(type,"real")==0){
					s->var_value.float_value = 0.0;
				}
				else if(strcmp(type,"boolean")==0){
					s->var_value.int_value = 0;
				}
				else if(strcmp(type,"array")==0){
					strcpy(s->var_value.string_value, "00000x54");
				}
				HASH_ADD_STR( SYMBOL_TABLE, var_name, s );  /* var_name: name of key field */
				//SYMBOL_TABLE->current_size++;
			}
			else
			{
				printf("Error : Variable '%s' already declared with '%s' type.\n",s->var_name, s->type);
				return 0;
			}
			var_name_stack[i] = NULL;
		}
		var_name_stack_top = -1;
		return 1;
	}

    int check_valid_identifier(char* var_name) {
		struct symbol_table *s = NULL;
		char var_mang_name[31];
		strcpy(var_mang_name, var_name);
		strcat(var_mang_name, "$");
		strcat(var_mang_name, curr_scope_level);
		HASH_FIND_STR(SYMBOL_TABLE, var_mang_name, s);
		if(!s)
			return 0;
		return 1;

	}

    union data get_identifier_data(char *var_name) {
		struct symbol_table *s = NULL;
		char var_mang_name[31];
		strcpy(var_mang_name, var_name);
		strcat(var_mang_name, "$");
		strcat(var_mang_name, curr_scope_level);
		HASH_FIND_STR(SYMBOL_TABLE, var_mang_name, s);
		return s->var_value;
	}

    int solution(int a,int b, char* operator) {
		int result;
		if(strcmp(operator,"+") == 0)
		{
			result = a+b;

		}
		if(strcmp(operator,"*") == 0)
		{
			result = a*b;

		}
		if(strcmp(operator,"/") == 0)
		{
			result = a/b;

		}
		if(strcmp(operator,"-") == 0)
		{
			result = a-b;

		}
		if(strcmp(operator,"%") == 0)
		{
			result = a%b;

		}
		return result;
	}
        

#line 221 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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

#line 372 "y.tab.c"

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
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_T_PROGRAM = 3,                  /* T_PROGRAM  */
  YYSYMBOL_T_IDENTIFIER = 4,               /* T_IDENTIFIER  */
  YYSYMBOL_T_USES = 5,                     /* T_USES  */
  YYSYMBOL_T_TYPE = 6,                     /* T_TYPE  */
  YYSYMBOL_T_VAR = 7,                      /* T_VAR  */
  YYSYMBOL_T_BEGIN = 8,                    /* T_BEGIN  */
  YYSYMBOL_T_END = 9,                      /* T_END  */
  YYSYMBOL_T_FUNCTION = 10,                /* T_FUNCTION  */
  YYSYMBOL_T_PROCEDURE = 11,               /* T_PROCEDURE  */
  YYSYMBOL_T_ARRAY = 12,                   /* T_ARRAY  */
  YYSYMBOL_T_OF = 13,                      /* T_OF  */
  YYSYMBOL_T_CONST = 14,                   /* T_CONST  */
  YYSYMBOL_T_IF = 15,                      /* T_IF  */
  YYSYMBOL_T_THEN = 16,                    /* T_THEN  */
  YYSYMBOL_T_ELSE = 17,                    /* T_ELSE  */
  YYSYMBOL_T_FOR = 18,                     /* T_FOR  */
  YYSYMBOL_T_TO = 19,                      /* T_TO  */
  YYSYMBOL_T_DOWNTO = 20,                  /* T_DOWNTO  */
  YYSYMBOL_T_DO = 21,                      /* T_DO  */
  YYSYMBOL_T_WRITE = 22,                   /* T_WRITE  */
  YYSYMBOL_T_WRITELN = 23,                 /* T_WRITELN  */
  YYSYMBOL_T_INDEXTYPE = 24,               /* T_INDEXTYPE  */
  YYSYMBOL_T_SINGLEEQ = 25,                /* T_SINGLEEQ  */
  YYSYMBOL_T_INTVAL = 26,                  /* T_INTVAL  */
  YYSYMBOL_T_BOOLVAL = 27,                 /* T_BOOLVAL  */
  YYSYMBOL_T_FLOATVAL = 28,                /* T_FLOATVAL  */
  YYSYMBOL_T_STRINGVAL = 29,               /* T_STRINGVAL  */
  YYSYMBOL_T_DATATYPE = 30,                /* T_DATATYPE  */
  YYSYMBOL_T_ASOP = 31,                    /* T_ASOP  */
  YYSYMBOL_T_AS_PE = 32,                   /* T_AS_PE  */
  YYSYMBOL_T_AS_SE = 33,                   /* T_AS_SE  */
  YYSYMBOL_T_AS_MULE = 34,                 /* T_AS_MULE  */
  YYSYMBOL_T_AS_DIVE = 35,                 /* T_AS_DIVE  */
  YYSYMBOL_T_GE = 36,                      /* T_GE  */
  YYSYMBOL_T_LE = 37,                      /* T_LE  */
  YYSYMBOL_T_NE = 38,                      /* T_NE  */
  YYSYMBOL_T_BOOL_AND = 39,                /* T_BOOL_AND  */
  YYSYMBOL_T_BOOL_OR = 40,                 /* T_BOOL_OR  */
  YYSYMBOL_T_BOOL_NOT = 41,                /* T_BOOL_NOT  */
  YYSYMBOL_T_BIT_LEFT_SHIFT = 42,          /* T_BIT_LEFT_SHIFT  */
  YYSYMBOL_T_BIT_RIGHT_SHIFT = 43,         /* T_BIT_RIGHT_SHIFT  */
  YYSYMBOL_44_ = 44,                       /* '+'  */
  YYSYMBOL_45_ = 45,                       /* '-'  */
  YYSYMBOL_46_ = 46,                       /* '*'  */
  YYSYMBOL_47_ = 47,                       /* '/'  */
  YYSYMBOL_48_ = 48,                       /* '.'  */
  YYSYMBOL_49_ = 49,                       /* ';'  */
  YYSYMBOL_50_ = 50,                       /* ','  */
  YYSYMBOL_51_ = 51,                       /* ':'  */
  YYSYMBOL_52_ = 52,                       /* '['  */
  YYSYMBOL_53_ = 53,                       /* ']'  */
  YYSYMBOL_54_ = 54,                       /* '('  */
  YYSYMBOL_55_ = 55,                       /* ')'  */
  YYSYMBOL_56_ = 56,                       /* '<'  */
  YYSYMBOL_57_ = 57,                       /* '>'  */
  YYSYMBOL_58_ = 58,                       /* '|'  */
  YYSYMBOL_59_ = 59,                       /* '!'  */
  YYSYMBOL_60_ = 60,                       /* '%'  */
  YYSYMBOL_61_ = 61,                       /* '&'  */
  YYSYMBOL_YYACCEPT = 62,                  /* $accept  */
  YYSYMBOL_startProg = 63,                 /* startProg  */
  YYSYMBOL_program = 64,                   /* program  */
  YYSYMBOL_programHeading = 65,            /* programHeading  */
  YYSYMBOL_66_1 = 66,                      /* $@1  */
  YYSYMBOL_block = 67,                     /* block  */
  YYSYMBOL_uses_block = 68,                /* uses_block  */
  YYSYMBOL_other_libs = 69,                /* other_libs  */
  YYSYMBOL_const_block = 70,               /* const_block  */
  YYSYMBOL_const_definition = 71,          /* const_definition  */
  YYSYMBOL_72_2 = 72,                      /* $@2  */
  YYSYMBOL_constant = 73,                  /* constant  */
  YYSYMBOL_more_const_definition = 74,     /* more_const_definition  */
  YYSYMBOL_75_3 = 75,                      /* $@3  */
  YYSYMBOL_type_block = 76,                /* type_block  */
  YYSYMBOL_type_definition = 77,           /* type_definition  */
  YYSYMBOL_78_4 = 78,                      /* $@4  */
  YYSYMBOL_79_5 = 79,                      /* $@5  */
  YYSYMBOL_more_type_identifiers = 80,     /* more_type_identifiers  */
  YYSYMBOL_81_6 = 81,                      /* $@6  */
  YYSYMBOL_variable_block = 82,            /* variable_block  */
  YYSYMBOL_decl_stmts = 83,                /* decl_stmts  */
  YYSYMBOL_84_7 = 84,                      /* $@7  */
  YYSYMBOL_more_decl_stmt = 85,            /* more_decl_stmt  */
  YYSYMBOL_86_8 = 86,                      /* $@8  */
  YYSYMBOL_data_type = 87,                 /* data_type  */
  YYSYMBOL_function_and_procedure_block = 88, /* function_and_procedure_block  */
  YYSYMBOL_procedure_block = 89,           /* procedure_block  */
  YYSYMBOL_90_9 = 90,                      /* $@9  */
  YYSYMBOL_91_10 = 91,                     /* $@10  */
  YYSYMBOL_param_list = 92,                /* param_list  */
  YYSYMBOL_function_block = 93,            /* function_block  */
  YYSYMBOL_94_11 = 94,                     /* $@11  */
  YYSYMBOL_95_12 = 95,                     /* $@12  */
  YYSYMBOL_function_param_list = 96,       /* function_param_list  */
  YYSYMBOL_97_13 = 97,                     /* $@13  */
  YYSYMBOL_98_14 = 98,                     /* $@14  */
  YYSYMBOL_function_param_continue = 99,   /* function_param_continue  */
  YYSYMBOL_more_func_identifiers = 100,    /* more_func_identifiers  */
  YYSYMBOL_101_15 = 101,                   /* $@15  */
  YYSYMBOL_execution_block = 102,          /* execution_block  */
  YYSYMBOL_execution_body = 103,           /* execution_body  */
  YYSYMBOL_print_statements = 104,         /* print_statements  */
  YYSYMBOL_assignment_statements = 105,    /* assignment_statements  */
  YYSYMBOL_106_16 = 106,                   /* $@16  */
  YYSYMBOL_expression = 107,               /* expression  */
  YYSYMBOL_simpleExpression = 108,         /* simpleExpression  */
  YYSYMBOL_term = 109,                     /* term  */
  YYSYMBOL_factor = 110,                   /* factor  */
  YYSYMBOL_value = 111,                    /* value  */
  YYSYMBOL_assignment_operators = 112,     /* assignment_operators  */
  YYSYMBOL_relational_operators = 113,     /* relational_operators  */
  YYSYMBOL_if_statement = 114,             /* if_statement  */
  YYSYMBOL_if_then_follow = 115,           /* if_then_follow  */
  YYSYMBOL_else_if_block = 116,            /* else_if_block  */
  YYSYMBOL_else_block = 117,               /* else_block  */
  YYSYMBOL_fordo_statement = 118,          /* fordo_statement  */
  YYSYMBOL_to_or_downto = 119,             /* to_or_downto  */
  YYSYMBOL_boolean_expression = 120        /* boolean_expression  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   221

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  62
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  59
/* YYNRULES -- Number of rules.  */
#define YYNRULES  121
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  229

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   298


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    59,     2,     2,     2,    60,    61,     2,
      54,    55,    46,    44,    50,    45,    48,    47,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    51,    49,
      56,     2,    57,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    52,     2,    53,     2,     2,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   228,   228,   232,   236,   236,   242,   246,   252,   256,
     261,   265,   266,   270,   270,   300,   303,   306,   312,   312,
     317,   321,   322,   327,   332,   326,   353,   358,   357,   363,
     367,   368,   374,   373,   379,   384,   383,   389,   393,   404,
     423,   436,   437,   438,   443,   442,   449,   448,   457,   458,
     459,   464,   463,   474,   473,   486,   491,   485,   502,   503,
     508,   507,   513,   517,   521,   522,   523,   524,   525,   529,
     534,   533,   552,   553,   554,   555,   556,   557,   558,   587,
     588,   589,   590,   591,   592,   621,   622,   623,   624,   625,
     626,   655,   656,   657,   658,   659,   660,   669,   691,   713,
     735,   760,   761,   762,   763,   764,   768,   769,   770,   771,
     772,   773,   776,   780,   781,   782,   786,   790,   794,   798,
     799,   803
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "T_PROGRAM",
  "T_IDENTIFIER", "T_USES", "T_TYPE", "T_VAR", "T_BEGIN", "T_END",
  "T_FUNCTION", "T_PROCEDURE", "T_ARRAY", "T_OF", "T_CONST", "T_IF",
  "T_THEN", "T_ELSE", "T_FOR", "T_TO", "T_DOWNTO", "T_DO", "T_WRITE",
  "T_WRITELN", "T_INDEXTYPE", "T_SINGLEEQ", "T_INTVAL", "T_BOOLVAL",
  "T_FLOATVAL", "T_STRINGVAL", "T_DATATYPE", "T_ASOP", "T_AS_PE",
  "T_AS_SE", "T_AS_MULE", "T_AS_DIVE", "T_GE", "T_LE", "T_NE",
  "T_BOOL_AND", "T_BOOL_OR", "T_BOOL_NOT", "T_BIT_LEFT_SHIFT",
  "T_BIT_RIGHT_SHIFT", "'+'", "'-'", "'*'", "'/'", "'.'", "';'", "','",
  "':'", "'['", "']'", "'('", "')'", "'<'", "'>'", "'|'", "'!'", "'%'",
  "'&'", "$accept", "startProg", "program", "programHeading", "$@1",
  "block", "uses_block", "other_libs", "const_block", "const_definition",
  "$@2", "constant", "more_const_definition", "$@3", "type_block",
  "type_definition", "$@4", "$@5", "more_type_identifiers", "$@6",
  "variable_block", "decl_stmts", "$@7", "more_decl_stmt", "$@8",
  "data_type", "function_and_procedure_block", "procedure_block", "$@9",
  "$@10", "param_list", "function_block", "$@11", "$@12",
  "function_param_list", "$@13", "$@14", "function_param_continue",
  "more_func_identifiers", "$@15", "execution_block", "execution_body",
  "print_statements", "assignment_statements", "$@16", "expression",
  "simpleExpression", "term", "factor", "value", "assignment_operators",
  "relational_operators", "if_statement", "if_then_follow",
  "else_if_block", "else_block", "fordo_statement", "to_or_downto",
  "boolean_expression", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,    43,    45,    42,    47,    46,    59,
      44,    58,    91,    93,    40,    41,    60,    62,   124,    33,
      37,    38
};
#endif

#define YYPACT_NINF (-198)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-54)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       4,     5,    23,  -198,    26,  -198,  -198,    31,     6,    45,
      13,    24,  -198,    78,    77,  -198,    81,    43,    75,  -198,
     101,    99,    24,  -198,   -15,  -198,  -198,   103,    11,  -198,
    -198,  -198,  -198,  -198,    58,  -198,  -198,   111,   112,   109,
      11,    11,    69,   115,    95,    71,    68,    70,  -198,  -198,
    -198,  -198,   119,  -198,    96,   121,    80,    82,    73,    83,
      74,    66,   104,  -198,    58,  -198,  -198,    57,   100,   130,
      26,   131,  -198,  -198,    85,   132,    87,  -198,  -198,  -198,
    -198,   -15,  -198,    88,    71,  -198,  -198,    90,    91,    94,
    -198,    89,    97,    98,    92,    79,    -1,   114,   122,  -198,
     101,  -198,   124,  -198,    26,   102,   105,  -198,   120,   106,
    -198,  -198,  -198,  -198,  -198,    -1,  -198,  -198,  -198,  -198,
    -198,    -1,    -1,    -1,    -1,    30,    20,   -31,  -198,  -198,
     107,    -1,   108,   110,  -198,   113,   116,   149,   117,   127,
     118,    26,  -198,  -198,  -198,  -198,   123,  -198,  -198,  -198,
    -198,  -198,  -198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    27,   125,   119,   145,  -198,  -198,   134,   126,   131,
     128,  -198,  -198,   -26,   -26,   -26,   -26,   -31,   -31,   -31,
     -26,   -26,   -31,   -31,  -198,  -198,  -198,  -198,  -198,  -198,
    -198,  -198,    -1,  -198,  -198,   139,   102,  -198,    26,  -198,
    -198,    76,   140,  -198,  -198,   133,   135,   155,  -198,  -198,
    -198,  -198,   130,  -198,  -198,    86,  -198,    86,  -198
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     2,     8,     4,     1,     0,     0,    12,
       0,    10,     3,     0,    22,     5,     0,     0,     0,    11,
      26,    31,    10,     7,     0,    23,    21,    34,    43,     9,
      15,    16,    17,    13,    29,    32,    30,     0,     0,     0,
      43,    43,     0,     0,     0,    37,    51,    44,    68,     6,
      42,    41,    20,    27,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    14,    29,    24,    35,     0,     0,     0,
       8,    50,    70,    63,     0,     0,     0,    67,    64,    65,
      66,     0,    28,     0,    37,    39,    38,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,     0,    18,
      26,    36,     0,    33,     8,    62,     0,    45,     0,     0,
     101,   102,   103,   104,   105,     0,    96,    97,    99,    98,
     100,     0,     0,     0,     0,     0,    72,    79,    85,    95,
       0,     0,     0,     0,    25,     0,     0,     0,     0,     0,
      48,     8,    71,    94,    92,    93,     0,   106,   109,   110,
     111,   108,   107,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    20,     0,    52,    60,     0,     0,    50,
       0,    91,   121,    73,    78,    76,    74,    82,    80,    81,
      75,    77,    83,    84,    89,    86,    87,    88,    90,    68,
     119,   120,     0,    69,    19,     0,    62,    56,     8,    49,
      47,   115,     0,    40,    61,    59,     0,    68,   112,   113,
     114,    68,     0,    57,    54,   117,   116,   118,    58
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -198,  -198,  -198,  -198,  -198,   -70,  -198,   150,  -198,  -198,
    -198,   129,   -13,  -198,  -198,    93,  -198,  -198,   136,  -198,
    -198,  -198,  -198,   137,  -198,  -198,    32,  -198,  -198,  -198,
      -8,  -198,  -198,  -198,   -49,  -198,  -198,  -198,   -30,  -198,
    -198,  -197,  -198,  -198,  -198,  -114,   -59,  -122,  -117,  -198,
    -198,  -198,   -38,  -198,  -198,  -198,  -198,  -198,  -198
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     2,     3,     4,    10,     8,     9,    17,    14,    19,
      42,    33,    63,   133,    21,    26,    34,    83,    44,    64,
      28,    36,    45,    56,    84,    88,    39,    40,    59,    60,
      94,    41,    57,    58,    91,   105,   215,   223,   138,   206,
      49,    61,    77,    78,    95,   125,   126,   127,   128,   129,
     115,   153,    79,   218,   219,   220,    80,   202,   130
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      92,   142,   211,   116,   143,   144,   145,     1,   165,     5,
     146,    30,    31,    32,   158,   166,   167,   171,   159,   160,
     225,    37,    38,     6,   227,   117,   118,   119,   120,   168,
     169,     7,   163,   164,   136,    11,   187,   188,   189,   182,
     121,   192,   193,   122,   123,   154,   200,   201,   194,   195,
     196,   197,   198,   124,    12,   147,   155,   156,   157,    13,
     158,    85,    15,    86,   159,   160,   148,   149,   150,    87,
      72,   180,    50,    51,    16,    73,   161,   162,   163,   164,
      72,    74,    18,    20,    75,    22,   151,   152,   212,    76,
      72,    74,    23,   217,    75,   183,   184,   185,   186,    76,
      24,    74,   190,   191,    75,    25,    27,    35,    43,    76,
     110,   111,   112,   113,   114,    46,    47,    48,    52,    53,
      54,    55,   -53,    62,   -46,    66,    65,    69,    71,    81,
      89,    67,    70,    68,    90,    93,    97,   100,   216,    96,
     103,    98,   102,   104,   106,   131,   107,   109,   135,   108,
     140,   132,   137,   176,   199,   141,   139,   178,   205,   173,
     204,   221,   170,   172,   207,   175,   174,   179,   177,   213,
      74,   209,    29,   228,   203,   208,   214,   210,   181,   226,
       0,     0,   222,     0,   224,     0,     0,     0,     0,     0,
       0,     0,     0,   134,     0,     0,     0,     0,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   101
};

static const yytype_int16 yycheck[] =
{
      70,   115,   199,     4,   121,   122,   123,     3,    39,     4,
     124,    26,    27,    28,    40,    46,    47,   131,    44,    45,
     217,    10,    11,     0,   221,    26,    27,    28,    29,    60,
      61,     5,    58,    59,   104,     4,   158,   159,   160,   153,
      41,   163,   164,    44,    45,    25,    19,    20,   165,   166,
     167,   168,   169,    54,    48,    25,    36,    37,    38,    14,
      40,     4,    49,     6,    44,    45,    36,    37,    38,    12,
       4,   141,    40,    41,    50,     9,    56,    57,    58,    59,
       4,    15,     4,     6,    18,     4,    56,    57,   202,    23,
       4,    15,    49,    17,    18,   154,   155,   156,   157,    23,
      25,    15,   161,   162,    18,     4,     7,     4,    50,    23,
      31,    32,    33,    34,    35,     4,     4,     8,    49,     4,
      25,    50,    54,     4,    54,     4,    30,    54,    54,    25,
      30,    51,    49,    51,     4,     4,     4,    49,   208,    54,
      49,    54,    52,    49,    55,    31,    49,    55,    24,    51,
      30,    29,    50,     4,    16,    49,    51,    30,    13,    49,
     173,    21,    55,    55,    30,    49,    53,    49,    51,    30,
      15,   179,    22,   222,    49,    49,   206,    49,    55,   217,
      -1,    -1,    49,    -1,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    63,    64,    65,     4,     0,     5,    67,    68,
      66,     4,    48,    14,    70,    49,    50,    69,     4,    71,
       6,    76,     4,    49,    25,     4,    77,     7,    82,    69,
      26,    27,    28,    73,    78,     4,    83,    10,    11,    88,
      89,    93,    72,    50,    80,    84,     4,     4,     8,   102,
      88,    88,    49,     4,    25,    50,    85,    94,    95,    90,
      91,   103,     4,    74,    81,    30,     4,    51,    51,    54,
      49,    54,     4,     9,    15,    18,    23,   104,   105,   114,
     118,    25,    80,    79,    86,     4,     6,    12,    87,    30,
       4,    96,    67,     4,    92,   106,    54,     4,    54,    73,
      49,    85,    52,    49,    49,    97,    55,    49,    51,    55,
      31,    32,    33,    34,    35,   112,     4,    26,    27,    28,
      29,    41,    44,    45,    54,   107,   108,   109,   110,   111,
     120,    31,    29,    75,    77,    24,    67,    50,   100,    51,
      30,    49,   107,   110,   110,   110,   107,    25,    36,    37,
      38,    56,    57,   113,    25,    36,    37,    38,    40,    44,
      45,    56,    57,    58,    59,    39,    46,    47,    60,    61,
      55,   107,    55,    49,    53,    49,     4,    51,    30,    49,
      67,    55,   107,   108,   108,   108,   108,   109,   109,   109,
     108,   108,   109,   109,   110,   110,   110,   110,   110,    16,
      19,    20,   119,    49,    74,    13,   101,    30,    49,    92,
      49,   103,   107,    30,   100,    98,    67,    17,   115,   116,
     117,    21,    49,    99,    49,   103,   114,   103,    96
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    62,    63,    64,    66,    65,    67,    68,    68,    69,
      69,    70,    70,    72,    71,    73,    73,    73,    75,    74,
      74,    76,    76,    78,    79,    77,    77,    81,    80,    80,
      82,    82,    84,    83,    83,    86,    85,    85,    87,    87,
      87,    88,    88,    88,    90,    89,    91,    89,    92,    92,
      92,    94,    93,    95,    93,    97,    98,    96,    99,    99,
     101,   100,   100,   102,   103,   103,   103,   103,   103,   104,
     106,   105,   107,   107,   107,   107,   107,   107,   107,   108,
     108,   108,   108,   108,   108,   109,   109,   109,   109,   109,
     109,   110,   110,   110,   110,   110,   110,   111,   111,   111,
     111,   112,   112,   112,   112,   112,   113,   113,   113,   113,
     113,   113,   114,   115,   115,   115,   116,   117,   118,   119,
     119,   120
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     3,     0,     4,     6,     4,     0,     3,
       0,     2,     0,     0,     6,     1,     1,     1,     0,     6,
       0,     2,     0,     0,     0,     8,     0,     0,     4,     0,
       2,     0,     0,     6,     0,     0,     4,     0,     1,     1,
       6,     2,     2,     0,     0,     6,     0,     9,     3,     5,
       0,     0,     8,     0,    11,     0,     0,     7,     2,     0,
       0,     4,     0,     3,     2,     2,     2,     2,     0,     5,
       0,     4,     1,     3,     3,     3,     3,     3,     3,     1,
       3,     3,     3,     3,     3,     1,     3,     3,     3,     3,
       3,     3,     2,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     7,     1,     1,     0,     2,     2,     8,     1,
       1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YY_LOCATION_PRINT
#  if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#   define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

#  else
#   define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#  endif
# endif /* !defined YY_LOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

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
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
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
      if (yytable_value_is_error (yyn))
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 4: /* $@1: %empty  */
#line 236 "pascal.y"
                               {printf("Here I am\n");}
#line 1762 "y.tab.c"
    break;

  case 7: /* uses_block: T_USES T_IDENTIFIER other_libs ';'  */
#line 246 "pascal.y"
                                          {
                // uses_identifier_top++;
		// uses_identifier_stack[uses_identifier_top] = strdup(yylval.s.str);
                // might need to edit so I'm leaving
                // ast may come here (line 195)
        }
#line 1773 "y.tab.c"
    break;

  case 9: /* other_libs: ',' T_IDENTIFIER other_libs  */
#line 256 "pascal.y"
                                    {
                // uses_identifier_top++;
		// uses_identifier_stack[uses_identifier_top] = strdup(yylval.s.str);
                // might need to edit so I'm leaving this one as well 
        }
#line 1783 "y.tab.c"
    break;

  case 13: /* $@2: %empty  */
#line 270 "pascal.y"
                                        { 			
		// printf("%s %s %d %.5f \n",$<s.str>1,$<s.type>1,$<s.intval>1,$<s.floatval>1); 
		printf("yylval: %s\n",yylval.s.str);
		struct symbol_table *s = NULL;
		HASH_FIND_STR(SYMBOL_TABLE,(yyvsp[-2].s.str), s);
		if(!s){
			s = malloc(sizeof(struct symbol_table));
			strcpy(s->var_name,yylval.s.str);
			strcpy(s->type,yylval.s.type);
			s->line_no = yylloc.first_line;
			s->col_no = yylloc.first_column;
			if(yylval.s.intval!=0){
				s->var_value.int_value = yylval.s.intval;
			}
			if(yylval.s.floatval!=0){
				s->var_value.float_value = yylval.s.floatval;
			}

			HASH_ADD_STR(SYMBOL_TABLE, var_name, s);
			printf("yayy\n");
		}else{
			printf("ono\n");
			}
		// printf("%s %s %d %f \n",$<s.str>2,$<s.type>2,$<s.intval>2,$<s.floatval>2); 
		// printf("%s %s %d %f \n",$<s.str>3,$<s.type>3,$<s.intval>3,$<s.floatval>3); 
		
		}
#line 1815 "y.tab.c"
    break;

  case 15: /* constant: T_INTVAL  */
#line 300 "pascal.y"
                {
			printf("%s %s %d %.5f \n",(yyvsp[0].s.str),(yyvsp[0].s.type),(yyvsp[0].s.intval),(yyvsp[0].s.floatval)); 
		}
#line 1823 "y.tab.c"
    break;

  case 16: /* constant: T_BOOLVAL  */
#line 303 "pascal.y"
                   {
			printf("%s %s %d %.5f \n",(yyvsp[0].s.str),(yyvsp[0].s.type),(yyvsp[0].s.intval),(yyvsp[0].s.floatval)); 
		}
#line 1831 "y.tab.c"
    break;

  case 17: /* constant: T_FLOATVAL  */
#line 306 "pascal.y"
                    {
			printf("%s %s %d %.5f \n",(yyvsp[0].s.str),(yyvsp[0].s.type),(yyvsp[0].s.intval),(yyvsp[0].s.floatval)); 
		}
#line 1839 "y.tab.c"
    break;

  case 18: /* $@3: %empty  */
#line 312 "pascal.y"
                                        {
			// var_name_stack_top++;
			// var_name_stack[var_name_stack_top] = strdup(yylval.s.str);
		}
#line 1848 "y.tab.c"
    break;

  case 23: /* $@4: %empty  */
#line 327 "pascal.y"
        {
            type_identifier_top++;
			type_identifier_stack[type_identifier_top] = strdup(yylval.s.str);
        }
#line 1857 "y.tab.c"
    break;

  case 24: /* $@5: %empty  */
#line 332 "pascal.y"
        {
            for(int i = 0; i <= type_identifier_top; i++)
			{
				struct type_table *s = NULL;
				HASH_FIND_STR(TYPE_TABLE, type_identifier_stack[i], s);
				if(!s)
				{
					s = malloc(sizeof(struct type_table));
					strcpy(s->user_defined_name, type_identifier_stack[i]);
					strcpy(s->actual_type_name, yylval.s.type);
					HASH_ADD_STR(TYPE_TABLE, user_defined_name, s);  /* var_name: name of key field */
				}
				else
				{
					YYABORT;
				}
				type_identifier_stack[i] = NULL;
			}
			type_identifier_top = -1;
        }
#line 1882 "y.tab.c"
    break;

  case 27: /* $@6: %empty  */
#line 358 "pascal.y"
        {
            type_identifier_top++;
			type_identifier_stack[type_identifier_top] = strdup(yylval.s.str);
        }
#line 1891 "y.tab.c"
    break;

  case 32: /* $@7: %empty  */
#line 374 "pascal.y"
        {
            var_name_stack_top++;
			var_name_stack[var_name_stack_top] = strdup(yylval.s.str);
        }
#line 1900 "y.tab.c"
    break;

  case 35: /* $@8: %empty  */
#line 384 "pascal.y"
        {
            type_identifier_top++;
			type_identifier_stack[type_identifier_top] = strdup(yylval.s.str);
        }
#line 1909 "y.tab.c"
    break;

  case 38: /* data_type: T_TYPE  */
#line 394 "pascal.y"
        {
			int result = dump_stack_in_symbol_table(yylval.s.type, yylloc.first_line, yylloc.first_column);
                if(!result){
                        yyerror(" abort, Variable already declared.");
                        exit(1);

                }

		}
#line 1923 "y.tab.c"
    break;

  case 39: /* data_type: T_IDENTIFIER  */
#line 405 "pascal.y"
        {
            struct type_table *t = NULL;
			HASH_FIND_STR(TYPE_TABLE,yylval.s.str,t);

			if(t)
			{
				int result = dump_stack_in_symbol_table(t->actual_type_name, yylloc.first_line, yylloc.first_column);
				if(!result){
				yyerror("abort Variable already declared.");
				exit(1);
				}
			}
			else
			{
				printf("Alert : Type %s is not defined.",yylval.s.str);
				YYABORT;
			}
        }
#line 1946 "y.tab.c"
    break;

  case 40: /* data_type: T_ARRAY '[' T_INDEXTYPE ']' T_OF T_DATATYPE  */
#line 424 "pascal.y"
                {
			//printf("Hit the type part of line %s\n", yylval.type);
			int result = dump_stack_in_symbol_table("array", yylloc.first_line, yylloc.first_column);
			if(!result){
				//printf("DumpBck in Variable: %d\n",result);
				yyerror("Abort: Variable already declared.");
				exit(1);
			}
		}
#line 1960 "y.tab.c"
    break;

  case 44: /* $@9: %empty  */
#line 443 "pascal.y"
                {
			curr_scope_level = strdup(yylval.s.str);
			printf("Entering the Procedure %s\n", curr_scope_level);
		}
#line 1969 "y.tab.c"
    break;

  case 46: /* $@10: %empty  */
#line 449 "pascal.y"
                {
			curr_scope_level = strdup(yylval.s.str);
			printf("Entering the Procedure %s\n", curr_scope_level);
		}
#line 1978 "y.tab.c"
    break;

  case 51: /* $@11: %empty  */
#line 464 "pascal.y"
                {
			curr_scope_level = strdup(yylval.s.str);
			printf("Entering the Function %s\n", curr_scope_level);
		}
#line 1987 "y.tab.c"
    break;

  case 52: /* function_block: T_FUNCTION T_IDENTIFIER $@11 ':' T_DATATYPE ';' block ';'  */
#line 469 "pascal.y"
                {
			strcpy(curr_scope_level,"global");

		}
#line 1996 "y.tab.c"
    break;

  case 53: /* $@12: %empty  */
#line 474 "pascal.y"
                {
			curr_scope_level = strdup(yylval.s.str);
		}
#line 2004 "y.tab.c"
    break;

  case 54: /* function_block: T_FUNCTION T_IDENTIFIER $@12 '(' function_param_list ')' ':' T_DATATYPE ';' block ';'  */
#line 478 "pascal.y"
                {
			char s[10] = "global";
			curr_scope_level = strdup(s);
		}
#line 2013 "y.tab.c"
    break;

  case 55: /* $@13: %empty  */
#line 486 "pascal.y"
                {
			var_name_stack_top++;
			var_name_stack[var_name_stack_top] = strdup(yylval.s.str);
		}
#line 2022 "y.tab.c"
    break;

  case 56: /* $@14: %empty  */
#line 491 "pascal.y"
                {
		int result = dump_stack_in_symbol_table(yylval.s.type, yylloc.first_line, yylloc.first_column);
		if(!result){
				yyerror("Abort: Variable already declared.");
				exit(1);
			}
		}
#line 2034 "y.tab.c"
    break;

  case 60: /* $@15: %empty  */
#line 508 "pascal.y"
                {
			var_name_stack_top++;
			var_name_stack[var_name_stack_top] = strdup(yylval.s.str);
		}
#line 2043 "y.tab.c"
    break;

  case 70: /* $@16: %empty  */
#line 534 "pascal.y"
        {
			if(!check_valid_identifier(yylval.s.str)){
				char error[1000];
				//printf("Scope Level : %s ",curr_scope_level);
				sprintf(error,"Abort: Variable %s is not declared.",yylval.s.str);
				yyerror(error);
				exit(1);
			}
			else
			{
				assignment_name_stack_top++;
				assignment_name_stack[assignment_name_stack_top] = strdup(yylval.s.str);
			}
        }
#line 2062 "y.tab.c"
    break;

  case 78: /* expression: simpleExpression T_GE simpleExpression  */
#line 559 "pascal.y"
        {
                // not sure what this is so I left      
            printf("%d and %d and %s\n",(yyvsp[-2].s.intval),(yyvsp[0].s.intval),(yyvsp[-1].s.str));
			(yyval.s.intval) = solution((yyvsp[-2].s.intval),(yyvsp[0].s.intval),(yyvsp[-1].s.str));
			
			struct symbol_table *s = NULL;
			char var_mang_name[31];
			strcpy(var_mang_name, assignment_name_stack[assignment_name_stack_top]);
			strcat(var_mang_name, "$");
			strcat(var_mang_name, curr_scope_level);
			HASH_FIND_STR(SYMBOL_TABLE, var_mang_name, s);
			if(s)
			{
				struct symbol_table *temp = NULL;
				struct symbol_table *r = NULL;
				temp = malloc(sizeof(struct symbol_table));
				strcat(temp->var_name, var_mang_name);
				strcpy(temp->type, s->type);
				temp->scope_level = s->scope_level;
				temp->line_no = s->line_no;
				temp->col_no = s->col_no;
				temp->var_value.int_value = (yyval.s.intval);
				HASH_REPLACE_STR(SYMBOL_TABLE, var_name, temp, r);  /* var_name: name of key field */
			}
        }
#line 2092 "y.tab.c"
    break;

  case 84: /* simpleExpression: simpleExpression '!' term  */
#line 593 "pascal.y"
                {
                // not sure what this is so I left      
            printf("%d and %d and %s\n",(yyvsp[-2].s.intval),(yyvsp[0].s.intval),(yyvsp[-1].s.str));
			(yyval.s.intval) = solution((yyvsp[-2].s.intval),(yyvsp[0].s.intval),(yyvsp[-1].s.str));
			
			struct symbol_table *s = NULL;
			char var_mang_name[31];
			strcpy(var_mang_name, assignment_name_stack[assignment_name_stack_top]);
			strcat(var_mang_name, "$");
			strcat(var_mang_name, curr_scope_level);
			HASH_FIND_STR(SYMBOL_TABLE, var_mang_name, s);
			if(s)
			{
				struct symbol_table *temp = NULL;
				struct symbol_table *r = NULL;
				temp = malloc(sizeof(struct symbol_table));
				strcat(temp->var_name, var_mang_name);
				strcpy(temp->type, s->type);
				temp->scope_level = s->scope_level;
				temp->line_no = s->line_no;
				temp->col_no = s->col_no;
				temp->var_value.int_value = (yyval.s.intval);
				HASH_REPLACE_STR(SYMBOL_TABLE, var_name, temp, r);  /* var_name: name of key field */
			}
        }
#line 2122 "y.tab.c"
    break;

  case 90: /* term: term '&' factor  */
#line 627 "pascal.y"
                {
                // not sure what this is so I left      
            printf("%d and %d and %s\n",(yyvsp[-2].s.intval),(yyvsp[0].s.intval),(yyvsp[-1].s.str));
			(yyval.s.intval) = solution((yyvsp[-2].s.intval),(yyvsp[0].s.intval),(yyvsp[-1].s.str));
			
			struct symbol_table *s = NULL;
			char var_mang_name[31];
			strcpy(var_mang_name, assignment_name_stack[assignment_name_stack_top]);
			strcat(var_mang_name, "$");
			strcat(var_mang_name, curr_scope_level);
			HASH_FIND_STR(SYMBOL_TABLE, var_mang_name, s);
			if(s)
			{
				struct symbol_table *temp = NULL;
				struct symbol_table *r = NULL;
				temp = malloc(sizeof(struct symbol_table));
				strcat(temp->var_name, var_mang_name);
				strcpy(temp->type, s->type);
				temp->scope_level = s->scope_level;
				temp->line_no = s->line_no;
				temp->col_no = s->col_no;
				temp->var_value.int_value = (yyval.s.intval);
				HASH_REPLACE_STR(SYMBOL_TABLE, var_name, temp, r);  /* var_name: name of key field */
			}
        }
#line 2152 "y.tab.c"
    break;

  case 96: /* factor: T_IDENTIFIER  */
#line 661 "pascal.y"
                {
			if(check_valid_identifier(yyval.s.str)) {
				union data variable_value = get_identifier_data(yylval.s.str);
				(yyval.s.intval) = variable_value.int_value;
			}
        }
#line 2163 "y.tab.c"
    break;

  case 97: /* value: T_INTVAL  */
#line 670 "pascal.y"
        {
            struct symbol_table *s = NULL;
			char var_mang_name[31];
			strcpy(var_mang_name, assignment_name_stack[assignment_name_stack_top]);
			strcat(var_mang_name, "$");
			strcat(var_mang_name, curr_scope_level);
			HASH_FIND_STR(SYMBOL_TABLE, var_mang_name, s);
			if(s)
			{
				struct symbol_table *temp = NULL;
				struct symbol_table *r = NULL;
				temp = malloc(sizeof(struct symbol_table));
				strcat(temp->var_name, var_mang_name);
				strcpy(temp->type, s->type);
				temp->scope_level = s->scope_level;
				temp->line_no = s->line_no;
				temp->col_no = s->col_no;
				temp->var_value.int_value = yylval.s.intval;
				HASH_REPLACE_STR( SYMBOL_TABLE, var_name, temp,r );  /* var_name: name of key field */
			}
        }
#line 2189 "y.tab.c"
    break;

  case 98: /* value: T_FLOATVAL  */
#line 692 "pascal.y"
        {
			struct symbol_table *s = NULL;
			char var_mang_name[31];
			strcpy(var_mang_name, assignment_name_stack[assignment_name_stack_top]);
			strcat(var_mang_name, "$");
			strcat(var_mang_name, curr_scope_level);
			HASH_FIND_STR(SYMBOL_TABLE, var_mang_name, s);
			if(s)
			{
				struct symbol_table *temp = NULL;
				struct symbol_table *r = NULL;
				temp = malloc(sizeof(struct symbol_table));
				strcat(temp->var_name, var_mang_name);
				strcpy(temp->type, s->type);
				temp->scope_level = s->scope_level;
				temp->line_no = s->line_no;
				temp->col_no = s->col_no;
				temp->var_value.float_value = yylval.s.floatval;
				HASH_REPLACE_STR( SYMBOL_TABLE, var_name, temp,r );  /* var_name: name of key field */
			}
        }
#line 2215 "y.tab.c"
    break;

  case 99: /* value: T_BOOLVAL  */
#line 714 "pascal.y"
        {
			struct symbol_table *s = NULL;
			char var_mang_name[31];
			strcpy(var_mang_name, assignment_name_stack[assignment_name_stack_top]);
			strcat(var_mang_name, "$");
			strcat(var_mang_name, curr_scope_level);
			HASH_FIND_STR(SYMBOL_TABLE, var_mang_name, s);
			if(s)
			{
				struct symbol_table *temp = NULL;
				struct symbol_table *r = NULL;
				temp = malloc(sizeof(struct symbol_table));
				strcat(temp->var_name, var_mang_name);
				strcpy(temp->type, s->type);
				temp->scope_level = s->scope_level;
				temp->line_no = s->line_no;
				temp->col_no = s->col_no;
				temp->var_value.int_value = yylval.s.intval;
				HASH_REPLACE_STR( SYMBOL_TABLE, var_name, temp,r );  /* var_name: name of key field */
			}
        }
#line 2241 "y.tab.c"
    break;

  case 100: /* value: T_STRINGVAL  */
#line 736 "pascal.y"
        {
			struct symbol_table *s = NULL;
			char var_mang_name[31];
			strcpy(var_mang_name, assignment_name_stack[assignment_name_stack_top]);
			strcat(var_mang_name, "$");
			strcat(var_mang_name, curr_scope_level);
			HASH_FIND_STR(SYMBOL_TABLE, var_mang_name, s);
			if(s)
			{
				struct symbol_table *temp = NULL;
				struct symbol_table *r = NULL;
				temp = malloc(sizeof(struct symbol_table));
				strcat(temp->var_name, var_mang_name);
				strcpy(temp->type, s->type);
				temp->scope_level = s->scope_level;
				temp->line_no = s->line_no;
				temp->col_no = s->col_no;
				strcpy(temp->var_value.string_value,yylval.s.str);
				HASH_REPLACE_STR( SYMBOL_TABLE, var_name, temp,r );  /* var_name: name of key field */
			}
        }
#line 2267 "y.tab.c"
    break;


#line 2271 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  yyerror_range[1] = yylloc;
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
                      yytoken, &yylval, &yylloc);
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

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


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 806 "pascal.y"

int yyerror(const char *message) {
	printf("\n\nInvalid Syntax:%d:%d Reason being %s\n",yylloc.first_line,yylloc.first_column,message);
	printf("Compilation Failed\n");
	successful=0;
	return 0;
}

int main(int argc,char* argv[]) {
	struct timespec start;
	struct timespec end;

	if (argc>1) {
		yyin = fopen(argv[1],"r");
		if(yyin == NULL) {
			perror("Error ");
			exit(1);
		}
	}
	else {
		yyin = stdin;
	}

	char extension[8] = ".output";
	char outputfile[40] = "output/";

	/*To Create Output File*/
	char *ptr = strtok(argv[1], "/");
	char *inputfile; 
	while(ptr != NULL)
	{
		inputfile = strdup(ptr);
		ptr = strtok(NULL, "/");
	}

	strcat(outputfile,inputfile);
	strcat(outputfile,extension);

	yyout = (FILE*)fopen(outputfile,"w+");
	/*End Create Output File*/

	clock_gettime(CLOCK_REALTIME, &start);
	yyparse();
	clock_gettime(CLOCK_REALTIME, &end);
	if(successful){
		printf("\n\nCompiled Successfully\n");
		printf("Took : %lf seconds\n", time_elapsed(&start, &end));

		printf("\n\nSymbol Table Current Size:%d\n",HASH_COUNT(SYMBOL_TABLE));
		printf("\n\nConst Table Current Size:%d\n",HASH_COUNT(CONST_TABLE));

		struct symbol_table *s;
		int i=0;
	    for(s=SYMBOL_TABLE,i=0; s != NULL,i<HASH_COUNT(SYMBOL_TABLE); s=s->hh.next,i++) {

	    	if(strcmp(s->type,"string")==0){
					printf("Index : %-10d\t Identifier : %-20s\t DataType : %-20s\t ScopeLevel : %-20s\t Line_no : %-10d\t Col_no : %-10d Value:%-10s\n",i,s->var_name,s->type, s->scope_level, s->line_no, s->col_no, s->var_value.string_value );
				}
				else if(strcmp(s->type,"integer")==0){
					printf("Index : %-10d\t Identifier : %-20s\t DataType : %-20s\t ScopeLevel : %-20s\t Line_no : %-10d\t Col_no : %-10d Value:%-10d\n",i,s->var_name,s->type, s->scope_level, s->line_no, s->col_no, s->var_value.int_value );
				}
				else if(strcmp(s->type,"real")==0){
					printf("Index : %-10d\t Identifier : %-20s\t DataType : %-20s\t ScopeLevel : %-20s\t Line_no : %-10d\t Col_no : %-10d Value:%-10f\n",i,s->var_name,s->type, s->scope_level, s->line_no, s->col_no, s->var_value.float_value );
				}
				else if(strcmp(s->type,"boolean")==0){
					printf("Index : %-10d\t Identifier : %-20s\t DataType : %-20s\t ScopeLevel : %-20s\t Line_no : %-10d\t Col_no : %-10d Value:%-10d\n",i,s->var_name,s->type, s->scope_level, s->line_no, s->col_no, s->var_value.int_value );
				}
				else {
					printf("Index : %-10d\t Identifier : %-20s\t DataType : %-20s\t ScopeLevel : %-20s\t Line_no : %-10d\t Col_no : %-10d Value:%-10s\n",i,s->var_name,s->type, s->scope_level, s->line_no, s->col_no, s->var_value.string_value );
				}

	    }

	}

}

double time_elapsed(struct timespec *start, struct timespec *end) {
	double t;
	t = (end->tv_sec - start->tv_sec); // diff in seconds
	t += (end->tv_nsec - start->tv_nsec) * 0.000000001; //diff in nanoseconds
	return t;
}
