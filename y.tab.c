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
		char* stringval;
		// struct ast_node * ast;
	}s;

#line 373 "y.tab.c"

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
  YYSYMBOL_block = 66,                     /* block  */
  YYSYMBOL_uses_block = 67,                /* uses_block  */
  YYSYMBOL_other_libs = 68,                /* other_libs  */
  YYSYMBOL_const_block = 69,               /* const_block  */
  YYSYMBOL_const_definition = 70,          /* const_definition  */
  YYSYMBOL_71_1 = 71,                      /* $@1  */
  YYSYMBOL_constant = 72,                  /* constant  */
  YYSYMBOL_more_const_definition = 73,     /* more_const_definition  */
  YYSYMBOL_74_2 = 74,                      /* $@2  */
  YYSYMBOL_type_block = 75,                /* type_block  */
  YYSYMBOL_type_definition = 76,           /* type_definition  */
  YYSYMBOL_77_3 = 77,                      /* $@3  */
  YYSYMBOL_78_4 = 78,                      /* $@4  */
  YYSYMBOL_more_type_identifiers = 79,     /* more_type_identifiers  */
  YYSYMBOL_80_5 = 80,                      /* $@5  */
  YYSYMBOL_variable_block = 81,            /* variable_block  */
  YYSYMBOL_decl_stmts = 82,                /* decl_stmts  */
  YYSYMBOL_83_6 = 83,                      /* $@6  */
  YYSYMBOL_more_decl_stmt = 84,            /* more_decl_stmt  */
  YYSYMBOL_85_7 = 85,                      /* $@7  */
  YYSYMBOL_data_type = 86,                 /* data_type  */
  YYSYMBOL_function_and_procedure_block = 87, /* function_and_procedure_block  */
  YYSYMBOL_procedure_block = 88,           /* procedure_block  */
  YYSYMBOL_89_8 = 89,                      /* $@8  */
  YYSYMBOL_90_9 = 90,                      /* $@9  */
  YYSYMBOL_param_list = 91,                /* param_list  */
  YYSYMBOL_function_block = 92,            /* function_block  */
  YYSYMBOL_93_10 = 93,                     /* $@10  */
  YYSYMBOL_94_11 = 94,                     /* $@11  */
  YYSYMBOL_function_param_list = 95,       /* function_param_list  */
  YYSYMBOL_96_12 = 96,                     /* $@12  */
  YYSYMBOL_97_13 = 97,                     /* $@13  */
  YYSYMBOL_function_param_continue = 98,   /* function_param_continue  */
  YYSYMBOL_more_func_identifiers = 99,     /* more_func_identifiers  */
  YYSYMBOL_100_14 = 100,                   /* $@14  */
  YYSYMBOL_execution_block = 101,          /* execution_block  */
  YYSYMBOL_statementList = 102,            /* statementList  */
  YYSYMBOL_statements = 103,               /* statements  */
  YYSYMBOL_print_statements = 104,         /* print_statements  */
  YYSYMBOL_assignment_statements = 105,    /* assignment_statements  */
  YYSYMBOL_106_15 = 106,                   /* $@15  */
  YYSYMBOL_expression = 107,               /* expression  */
  YYSYMBOL_simpleExpression = 108,         /* simpleExpression  */
  YYSYMBOL_term = 109,                     /* term  */
  YYSYMBOL_factor = 110,                   /* factor  */
  YYSYMBOL_value = 111,                    /* value  */
  YYSYMBOL_assignment_operators = 112,     /* assignment_operators  */
  YYSYMBOL_relational_operators = 113,     /* relational_operators  */
  YYSYMBOL_if_statement = 114,             /* if_statement  */
  YYSYMBOL_fordo_statement = 115,          /* fordo_statement  */
  YYSYMBOL_to_or_downto = 116,             /* to_or_downto  */
  YYSYMBOL_boolean_expression = 117        /* boolean_expression  */
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
#define YYLAST   215

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  62
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  56
/* YYNRULES -- Number of rules.  */
#define YYNRULES  120
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
       0,   229,   229,   233,   237,   243,   247,   253,   257,   262,
     266,   267,   271,   271,   301,   302,   303,   304,   308,   308,
     313,   317,   318,   323,   328,   322,   349,   354,   353,   359,
     363,   364,   370,   369,   375,   380,   379,   385,   389,   400,
     419,   432,   433,   434,   439,   438,   445,   444,   453,   454,
     455,   460,   459,   470,   469,   482,   487,   481,   498,   499,
     504,   503,   509,   513,   517,   518,   522,   523,   524,   525,
     526,   527,   531,   536,   535,   554,   555,   556,   557,   558,
     559,   560,   589,   590,   591,   592,   593,   594,   623,   624,
     625,   626,   627,   628,   657,   658,   659,   660,   661,   662,
     671,   693,   715,   737,   762,   763,   764,   765,   766,   770,
     771,   772,   773,   774,   775,   778,   779,   783,   787,   788,
     792
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
  "'&'", "$accept", "startProg", "program", "programHeading", "block",
  "uses_block", "other_libs", "const_block", "const_definition", "$@1",
  "constant", "more_const_definition", "$@2", "type_block",
  "type_definition", "$@3", "$@4", "more_type_identifiers", "$@5",
  "variable_block", "decl_stmts", "$@6", "more_decl_stmt", "$@7",
  "data_type", "function_and_procedure_block", "procedure_block", "$@8",
  "$@9", "param_list", "function_block", "$@10", "$@11",
  "function_param_list", "$@12", "$@13", "function_param_continue",
  "more_func_identifiers", "$@14", "execution_block", "statementList",
  "statements", "print_statements", "assignment_statements", "$@15",
  "expression", "simpleExpression", "term", "factor", "value",
  "assignment_operators", "relational_operators", "if_statement",
  "fordo_statement", "to_or_downto", "boolean_expression", YY_NULLPTR
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

#define YYPACT_NINF (-194)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-54)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       1,     5,    11,  -194,    14,   -36,  -194,    17,   -19,    36,
    -194,    13,  -194,    57,    62,    65,    21,    49,  -194,    71,
      70,    13,  -194,    83,  -194,  -194,    76,    16,  -194,  -194,
    -194,  -194,  -194,  -194,    32,  -194,  -194,    79,    80,    85,
      16,    16,    42,    96,    88,    51,    60,    61,    58,  -194,
    -194,  -194,   112,  -194,    87,   114,    68,    69,    67,    74,
      72,  -194,    75,   118,    77,  -194,   115,    78,  -194,  -194,
    -194,  -194,   100,  -194,    32,  -194,  -194,     8,    98,   126,
      14,   128,    73,     4,   102,   106,  -194,    58,    83,  -194,
      89,    51,  -194,    84,  -194,    90,    91,  -194,    82,    92,
      93,    94,  -194,  -194,  -194,  -194,  -194,     4,  -194,  -194,
    -194,  -194,  -194,     4,     4,     4,     4,     3,   -22,    18,
    -194,  -194,    95,     4,    97,  -194,  -194,    71,  -194,   119,
    -194,    14,   101,   103,  -194,   116,    99,  -194,  -194,  -194,
    -194,   104,  -194,  -194,  -194,  -194,  -194,  -194,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,   129,    24,   107,   108,  -194,
     105,   111,   138,   110,   117,   113,    14,  -194,  -194,    27,
      27,    27,    27,    18,    18,    18,    27,    27,    18,    18,
    -194,  -194,  -194,  -194,  -194,    58,  -194,  -194,     4,  -194,
     112,   140,  -194,  -194,   125,   120,   128,   121,   146,   143,
    -194,   135,   101,  -194,    14,  -194,  -194,    58,    58,  -194,
    -194,   122,   123,  -194,  -194,   126,  -194,  -194,  -194
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     2,     7,     0,     1,     0,     0,    11,
       4,     9,     3,     0,    22,     0,     0,     0,    10,    26,
      31,     9,     6,     0,    23,    21,    34,    43,     8,    14,
      15,    16,    17,    12,    29,    32,    30,     0,     0,     0,
      43,    43,     0,     0,     0,    37,    51,    44,    71,     5,
      42,    41,    20,    27,     0,     0,     0,     0,     0,     0,
       0,    73,     0,     0,     0,    66,     0,    64,    70,    67,
      68,    69,     0,    13,    29,    24,    35,     0,     0,     0,
       7,    50,     0,     0,     0,     0,    63,    71,     0,    28,
       0,    37,    39,     0,    38,     0,     0,    55,     0,     0,
       0,     0,   104,   105,   106,   107,   108,     0,    99,   100,
     102,   101,   103,     0,     0,     0,     0,     0,    75,    82,
      88,    98,     0,     0,     0,    65,    18,    26,    36,     0,
      33,     7,    62,     0,    45,     0,     0,    74,    97,    95,
      96,     0,   109,   112,   113,   114,   111,   110,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    25,
       0,     0,     0,     0,     0,    48,     7,    94,   120,    76,
      81,    79,    77,    85,    83,    84,    78,    80,    86,    87,
      92,    89,    90,    91,    93,    71,   118,   119,     0,    72,
      20,     0,    52,    60,     0,     0,    50,     0,   116,     0,
      19,     0,    62,    56,     7,    49,    47,    71,    71,    40,
      61,    59,     0,   115,   117,     0,    57,    54,    58
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -194,  -194,  -194,  -194,   -80,  -194,   145,  -194,  -194,  -194,
      86,   -33,  -194,  -194,    41,  -194,  -194,   109,  -194,  -194,
    -194,  -194,   124,  -194,  -194,     6,  -194,  -194,  -194,   -31,
    -194,  -194,  -194,   -52,  -194,  -194,  -194,   -35,  -194,   137,
     127,  -193,  -194,  -194,  -194,  -106,   -62,   -56,  -108,  -194,
    -194,  -194,  -194,  -194,  -194,  -194
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     2,     3,     4,     8,     9,    16,    14,    18,    42,
      33,    73,   168,    20,    25,    34,    90,    44,    74,    27,
      36,    45,    56,    91,    95,    39,    40,    59,    60,   101,
      41,    57,    58,    98,   132,   221,   226,   173,   212,    65,
      66,    67,    68,    69,    82,   117,   118,   119,   120,   121,
     107,   148,    70,    71,   198,   122
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      99,   137,   208,   149,     1,   138,   139,   140,   108,     5,
     141,     6,    92,    10,   150,   151,   152,   166,   153,     7,
      93,    11,   154,   155,   223,   224,    37,    38,   142,    12,
     109,   110,   111,   112,   156,   157,   158,   159,    94,   143,
     144,   145,   178,   196,   197,   113,    50,    51,   114,   115,
      13,   171,   190,   191,   192,   193,   194,   160,   116,   146,
     147,    17,    61,    15,   161,   162,    48,   153,    19,    21,
      22,   154,   155,    62,    23,    24,    63,    26,   163,   164,
      35,    64,    43,    46,    47,   158,   159,   179,   180,   181,
     182,    52,   209,    48,   186,   187,   207,   183,   184,   185,
      53,    55,   188,   189,   102,   103,   104,   105,   106,    29,
      30,    31,    32,    54,   -53,   -46,    72,    75,    76,    77,
      78,    79,    84,    80,    86,    88,    81,    87,    96,    83,
      97,    85,   100,   123,   222,   124,   129,   133,   127,   130,
     131,   134,   203,   170,   135,   195,   175,   205,   176,   136,
     165,   172,   167,   211,   174,   213,   199,   200,   201,   177,
     202,   204,   206,   217,   218,   219,    28,   210,   169,   214,
     216,   225,   227,   228,   126,   215,    49,   220,     0,     0,
       0,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   125,   128
};

static const yytype_int16 yycheck[] =
{
      80,   107,   195,    25,     3,   113,   114,   115,     4,     4,
     116,     0,     4,    49,    36,    37,    38,   123,    40,     5,
      12,     4,    44,    45,   217,   218,    10,    11,    25,    48,
      26,    27,    28,    29,    56,    57,    58,    59,    30,    36,
      37,    38,   148,    19,    20,    41,    40,    41,    44,    45,
      14,   131,   160,   161,   162,   163,   164,    39,    54,    56,
      57,     4,     4,    50,    46,    47,     8,    40,     6,     4,
      49,    44,    45,    15,    25,     4,    18,     7,    60,    61,
       4,    23,    50,     4,     4,    58,    59,   149,   150,   151,
     152,    49,   198,     8,   156,   157,   176,   153,   154,   155,
       4,    50,   158,   159,    31,    32,    33,    34,    35,    26,
      27,    28,    29,    25,    54,    54,     4,    30,     4,    51,
      51,    54,     4,    49,     9,    25,    54,    49,    30,    54,
       4,    54,     4,    31,   214,    29,    52,    55,    49,    49,
      49,    49,     4,    24,    51,    16,    30,    30,    49,    55,
      55,    50,    55,    13,    51,    30,    49,    49,    53,    55,
      49,    51,    49,    17,    21,    30,    21,   200,   127,    49,
      49,    49,    49,   225,    88,   206,    39,   212,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    91
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    63,    64,    65,     4,     0,     5,    66,    67,
      49,     4,    48,    14,    69,    50,    68,     4,    70,     6,
      75,     4,    49,    25,     4,    76,     7,    81,    68,    26,
      27,    28,    29,    72,    77,     4,    82,    10,    11,    87,
      88,    92,    71,    50,    79,    83,     4,     4,     8,   101,
      87,    87,    49,     4,    25,    50,    84,    93,    94,    89,
      90,     4,    15,    18,    23,   101,   102,   103,   104,   105,
     114,   115,     4,    73,    80,    30,     4,    51,    51,    54,
      49,    54,   106,    54,     4,    54,     9,    49,    25,    79,
      78,    85,     4,    12,    30,    86,    30,     4,    95,    66,
       4,    91,    31,    32,    33,    34,    35,   112,     4,    26,
      27,    28,    29,    41,    44,    45,    54,   107,   108,   109,
     110,   111,   117,    31,    29,   102,    72,    49,    84,    52,
      49,    49,    96,    55,    49,    51,    55,   107,   110,   110,
     110,   107,    25,    36,    37,    38,    56,    57,   113,    25,
      36,    37,    38,    40,    44,    45,    56,    57,    58,    59,
      39,    46,    47,    60,    61,    55,   107,    55,    74,    76,
      24,    66,    50,    99,    51,    30,    49,    55,   107,   108,
     108,   108,   108,   109,   109,   109,   108,   108,   109,   109,
     110,   110,   110,   110,   110,    16,    19,    20,   116,    49,
      49,    53,    49,     4,    51,    30,    49,    66,   103,   107,
      73,    13,   100,    30,    49,    91,    49,    17,    21,    30,
      99,    97,    66,   103,   103,    49,    98,    49,    95
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    62,    63,    64,    65,    66,    67,    67,    68,    68,
      69,    69,    71,    70,    72,    72,    72,    72,    74,    73,
      73,    75,    75,    77,    78,    76,    76,    80,    79,    79,
      81,    81,    83,    82,    82,    85,    84,    84,    86,    86,
      86,    87,    87,    87,    89,    88,    90,    88,    91,    91,
      91,    93,    92,    94,    92,    96,    97,    95,    98,    98,
     100,    99,    99,   101,   102,   102,   103,   103,   103,   103,
     103,   103,   104,   106,   105,   107,   107,   107,   107,   107,
     107,   107,   108,   108,   108,   108,   108,   108,   109,   109,
     109,   109,   109,   109,   110,   110,   110,   110,   110,   110,
     111,   111,   111,   111,   112,   112,   112,   112,   112,   113,
     113,   113,   113,   113,   113,   114,   114,   115,   116,   116,
     117
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     3,     3,     6,     4,     0,     3,     0,
       2,     0,     0,     6,     1,     1,     1,     1,     0,     6,
       0,     2,     0,     0,     0,     8,     0,     0,     4,     0,
       2,     0,     0,     6,     0,     0,     4,     0,     1,     1,
       6,     2,     2,     0,     0,     6,     0,     9,     3,     5,
       0,     0,     8,     0,    11,     0,     0,     7,     2,     0,
       0,     4,     0,     3,     1,     3,     1,     1,     1,     1,
       1,     0,     5,     0,     4,     1,     3,     3,     3,     3,
       3,     3,     1,     3,     3,     3,     3,     3,     1,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     8,     6,     8,     1,     1,
       3
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
  case 6: /* uses_block: T_USES T_IDENTIFIER other_libs ';'  */
#line 247 "pascal.y"
                                          {
                // uses_identifier_top++;
		// uses_identifier_stack[uses_identifier_top] = strdup(yylval.s.str);
                // might need to edit so I'm leaving
                // ast may come here (line 195)
        }
#line 1762 "y.tab.c"
    break;

  case 8: /* other_libs: ',' T_IDENTIFIER other_libs  */
#line 257 "pascal.y"
                                    {
                // uses_identifier_top++;
		// uses_identifier_stack[uses_identifier_top] = strdup(yylval.s.str);
                // might need to edit so I'm leaving this one as well 
        }
#line 1772 "y.tab.c"
    break;

  case 12: /* $@1: %empty  */
#line 271 "pascal.y"
                                        { 			
		printf("%s %s %d %.5f %s \n",(yyvsp[-2].s.str),(yyvsp[-2].s.type),(yyvsp[-2].s.intval),(yyvsp[0].s.floatval),(yyvsp[-2].s.stringval)); 
		// printf("yylval: %s\n",yylval.s.str);
		struct symbol_table *s = NULL;
		HASH_FIND_STR(SYMBOL_TABLE,(yyvsp[-2].s.str), s);
		if(!s){
			s = malloc(sizeof(struct symbol_table));
			strcpy(s->var_name,yylval.s.str);
			strcpy(s->type,"const");
			s->line_no = yylloc.first_line;
			s->col_no = yylloc.first_column;
			if(yylval.s.intval!=0){
				s->var_value.int_value = (yyvsp[0].s.intval);
			}
			if(yylval.s.floatval!=0){
				s->var_value.float_value = (yyvsp[0].s.floatval);
			}

			HASH_ADD_STR(SYMBOL_TABLE, var_name, s);
			printf("yayy\n");
		}else{
			printf("ono\n");
			}
		// printf("%s %s %d %f \n",$<s.str>2,$<s.type>2,$<s.intval>2,$<s.floatval>2); 
		// printf("%s %s %d %f \n",$<s.str>3,$<s.type>3,$<s.intval>3,$<s.floatval>3); 
		
		}
#line 1804 "y.tab.c"
    break;

  case 18: /* $@2: %empty  */
#line 308 "pascal.y"
                                        {
			// var_name_stack_top++;
			// var_name_stack[var_name_stack_top] = strdup(yylval.s.str);
		}
#line 1813 "y.tab.c"
    break;

  case 23: /* $@3: %empty  */
#line 323 "pascal.y"
        {
            type_identifier_top++;
			type_identifier_stack[type_identifier_top] = strdup(yylval.s.str);
        }
#line 1822 "y.tab.c"
    break;

  case 24: /* $@4: %empty  */
#line 328 "pascal.y"
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
#line 1847 "y.tab.c"
    break;

  case 27: /* $@5: %empty  */
#line 354 "pascal.y"
        {
            type_identifier_top++;
			type_identifier_stack[type_identifier_top] = strdup(yylval.s.str);
        }
#line 1856 "y.tab.c"
    break;

  case 32: /* $@6: %empty  */
#line 370 "pascal.y"
        {
            var_name_stack_top++;
			var_name_stack[var_name_stack_top] = strdup(yylval.s.str);
        }
#line 1865 "y.tab.c"
    break;

  case 35: /* $@7: %empty  */
#line 380 "pascal.y"
        {
            var_name_stack_top++;
			var_name_stack[var_name_stack_top] = strdup(yylval.s.str);
        }
#line 1874 "y.tab.c"
    break;

  case 38: /* data_type: T_DATATYPE  */
#line 390 "pascal.y"
        {
			int result = dump_stack_in_symbol_table(yylval.s.type, yylloc.first_line, yylloc.first_column);
			if(!result){
					yyerror(" abort, Variable already declared.");
					exit(1);

			}

		}
#line 1888 "y.tab.c"
    break;

  case 39: /* data_type: T_IDENTIFIER  */
#line 401 "pascal.y"
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
#line 1911 "y.tab.c"
    break;

  case 40: /* data_type: T_ARRAY '[' T_INDEXTYPE ']' T_OF T_DATATYPE  */
#line 420 "pascal.y"
                {
			//printf("Hit the type part of line %s\n", yylval.type);
			int result = dump_stack_in_symbol_table("array", yylloc.first_line, yylloc.first_column);
			if(!result){
				//printf("DumpBck in Variable: %d\n",result);
				yyerror("Abort: Variable already declared.");
				exit(1);
			}
		}
#line 1925 "y.tab.c"
    break;

  case 44: /* $@8: %empty  */
#line 439 "pascal.y"
                {
			curr_scope_level = strdup(yylval.s.str);
			printf("Entering the Procedure %s\n", curr_scope_level);
		}
#line 1934 "y.tab.c"
    break;

  case 46: /* $@9: %empty  */
#line 445 "pascal.y"
                {
			curr_scope_level = strdup(yylval.s.str);
			printf("Entering the Procedure %s\n", curr_scope_level);
		}
#line 1943 "y.tab.c"
    break;

  case 51: /* $@10: %empty  */
#line 460 "pascal.y"
                {
			curr_scope_level = strdup(yylval.s.str);
			printf("Entering the Function %s\n", curr_scope_level);
		}
#line 1952 "y.tab.c"
    break;

  case 52: /* function_block: T_FUNCTION T_IDENTIFIER $@10 ':' T_DATATYPE ';' block ';'  */
#line 465 "pascal.y"
                {
			strcpy(curr_scope_level,"global");

		}
#line 1961 "y.tab.c"
    break;

  case 53: /* $@11: %empty  */
#line 470 "pascal.y"
                {
			curr_scope_level = strdup(yylval.s.str);
		}
#line 1969 "y.tab.c"
    break;

  case 54: /* function_block: T_FUNCTION T_IDENTIFIER $@11 '(' function_param_list ')' ':' T_DATATYPE ';' block ';'  */
#line 474 "pascal.y"
                {
			char s[10] = "global";
			curr_scope_level = strdup(s);
		}
#line 1978 "y.tab.c"
    break;

  case 55: /* $@12: %empty  */
#line 482 "pascal.y"
                {
			var_name_stack_top++;
			var_name_stack[var_name_stack_top] = strdup(yylval.s.str);
		}
#line 1987 "y.tab.c"
    break;

  case 56: /* $@13: %empty  */
#line 487 "pascal.y"
                {
		int result = dump_stack_in_symbol_table(yylval.s.type, yylloc.first_line, yylloc.first_column);
		if(!result){
				yyerror("Abort: Variable already declared.");
				exit(1);
			}
		}
#line 1999 "y.tab.c"
    break;

  case 60: /* $@14: %empty  */
#line 504 "pascal.y"
                {
			var_name_stack_top++;
			var_name_stack[var_name_stack_top] = strdup(yylval.s.str);
		}
#line 2008 "y.tab.c"
    break;

  case 73: /* $@15: %empty  */
#line 536 "pascal.y"
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
#line 2027 "y.tab.c"
    break;

  case 81: /* expression: simpleExpression T_GE simpleExpression  */
#line 561 "pascal.y"
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
#line 2057 "y.tab.c"
    break;

  case 87: /* simpleExpression: simpleExpression '!' term  */
#line 595 "pascal.y"
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
#line 2087 "y.tab.c"
    break;

  case 93: /* term: term '&' factor  */
#line 629 "pascal.y"
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
#line 2117 "y.tab.c"
    break;

  case 99: /* factor: T_IDENTIFIER  */
#line 663 "pascal.y"
                {
			if(check_valid_identifier(yyval.s.str)) {
				union data variable_value = get_identifier_data(yylval.s.str);
				(yyval.s.intval) = variable_value.int_value;
			}
        }
#line 2128 "y.tab.c"
    break;

  case 100: /* value: T_INTVAL  */
#line 672 "pascal.y"
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
#line 2154 "y.tab.c"
    break;

  case 101: /* value: T_FLOATVAL  */
#line 694 "pascal.y"
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
#line 2180 "y.tab.c"
    break;

  case 102: /* value: T_BOOLVAL  */
#line 716 "pascal.y"
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
#line 2206 "y.tab.c"
    break;

  case 103: /* value: T_STRINGVAL  */
#line 738 "pascal.y"
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
#line 2232 "y.tab.c"
    break;


#line 2236 "y.tab.c"

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

#line 795 "pascal.y"

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
