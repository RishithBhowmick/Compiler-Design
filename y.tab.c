/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

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
    #include "../uthash/src/uthash.h"
    #include "symbol_table.h" 
    #define YYPARSE_PARAM scanner
    #define YYLEX_PARAM   scanner
    int yylex();
    void yyerror(char *s);
    int successful=1;

    extern FILE *yyin;
    extern FILE *yyout;
    double time_elapsed(struct timespec *start, struct timespec *end);
    
    union data {    // since value is not of a single type. Union is the best structure. SO COOOOL
		int int_value;
		float float_value;
		char string_value[256];
    };
	char *var_name_stack[100];
	int var_name_stack_top = -1;

	char *assignment_name_stack[31];
	int assignment_name_stack_top = -1;
	
	char *curr_scope_level = "global";

	struct variable_type_info var_type_information;
	
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
        

#line 217 "y.tab.c"

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

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
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
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

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
#define YYLAST   201

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  63
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  56
/* YYNRULES -- Number of rules.  */
#define YYNRULES  118
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  226

#define YYUNDEFTOK  2
#define YYMAXUTOK   299


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    60,     2,     2,     2,    61,    62,     2,
      55,    56,    47,    45,    51,    46,    49,    48,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    52,    50,
      57,     2,    58,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    53,     2,    54,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    59,     2,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   213,   213,   217,   221,   227,   231,   237,   241,   246,
     250,   251,   255,   259,   260,   261,   265,   266,   270,   271,
     276,   281,   275,   302,   307,   306,   312,   316,   317,   323,
     322,   328,   333,   332,   338,   342,   353,   372,   385,   386,
     387,   392,   391,   398,   397,   406,   407,   408,   413,   412,
     423,   422,   435,   440,   434,   451,   452,   457,   456,   462,
     466,   470,   471,   472,   473,   474,   478,   483,   482,   501,
     502,   503,   504,   505,   506,   507,   536,   537,   538,   539,
     540,   541,   570,   571,   572,   573,   574,   575,   604,   605,
     606,   607,   608,   609,   618,   640,   662,   684,   709,   710,
     711,   712,   713,   717,   718,   719,   720,   721,   722,   725,
     729,   730,   731,   735,   739,   743,   747,   748,   752
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_PROGRAM", "T_IDENTIFIER", "T_USES",
  "T_TYPE", "T_VAR", "T_BEGIN", "T_END", "T_FUNCTION", "T_PROCEDURE",
  "T_AND", "T_ARRAY", "T_OF", "T_CONST", "T_IF", "T_THEN", "T_ELSE",
  "T_FOR", "T_TO", "T_DOWNTO", "T_DO", "T_WRITE", "T_WRITELN",
  "T_INDEXTYPE", "T_SINGLEEQ", "T_INTVAL", "T_BOOLVAL", "T_FLOATVAL",
  "T_STRINGVAL", "T_DATATYPE", "T_ASOP", "T_AS_PE", "T_AS_SE", "T_AS_MULE",
  "T_AS_DIVE", "T_GE", "T_LE", "T_NE", "T_BOOL_AND", "T_BOOL_OR",
  "T_BOOL_NOT", "T_BIT_LEFT_SHIFT", "T_BIT_RIGHT_SHIFT", "'+'", "'-'",
  "'*'", "'/'", "'.'", "';'", "','", "':'", "'['", "']'", "'('", "')'",
  "'<'", "'>'", "'|'", "'!'", "'%'", "'&'", "$accept", "startProg",
  "program", "programHeading", "block", "uses_block", "other_libs",
  "const_block", "const_definition", "constant", "more_const_definition",
  "type_block", "type_definition", "$@1", "$@2", "more_type_identifiers",
  "$@3", "variable_block", "decl_stmts", "$@4", "more_decl_stmt", "$@5",
  "data_type", "function_and_procedure_block", "procedure_block", "$@6",
  "$@7", "param_list", "function_block", "$@8", "$@9",
  "function_param_list", "$@10", "$@11", "function_param_continue",
  "more_func_identifiers", "$@12", "execution_block", "execution_body",
  "print_statements", "assignment_statements", "$@13", "expression",
  "simpleExpression", "term", "factor", "value", "assignment_operators",
  "relational_operators", "if_statement", "if_then_follow",
  "else_if_block", "else_block", "fordo_statement", "to_or_downto",
  "boolean_expression", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,    43,    45,    42,    47,    46,
      59,    44,    58,    91,    93,    40,    41,    60,    62,   124,
      33,    37,    38
};
# endif

#define YYPACT_NINF (-184)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-51)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      29,    16,    22,  -184,    37,     2,  -184,    42,    10,    47,
    -184,    12,  -184,    76,    80,    84,    41,    72,  -184,    95,
      93,    12,  -184,    39,  -184,  -184,   102,    -4,  -184,  -184,
    -184,  -184,    57,    64,  -184,  -184,   112,   113,   110,    -4,
      -4,   115,   116,    96,    70,    68,    69,  -184,  -184,  -184,
    -184,    99,  -184,  -184,    97,   122,    75,    77,    79,    81,
      82,    66,    39,    64,  -184,  -184,     5,   101,   126,    37,
     129,  -184,  -184,    83,   131,    85,  -184,  -184,  -184,  -184,
      89,  -184,    91,    70,  -184,  -184,    90,    92,    94,  -184,
      98,   100,   103,   104,    78,    -2,   114,   117,   115,    95,
    -184,   120,  -184,    37,   105,   106,  -184,   118,   107,  -184,
    -184,  -184,  -184,  -184,    -2,  -184,  -184,  -184,  -184,  -184,
      -2,    -2,    -2,    -2,    35,    19,   -32,  -184,  -184,   108,
      -2,   109,  -184,  -184,   119,   111,   144,   123,   121,   124,
      37,  -184,  -184,  -184,  -184,   125,  -184,  -184,  -184,  -184,
    -184,  -184,    -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,
      -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,   134,
      -8,   127,   139,  -184,  -184,   128,   130,   129,   132,  -184,
    -184,   -22,   -22,   -22,   -22,   -32,   -32,   -32,   -22,   -22,
     -32,   -32,  -184,  -184,  -184,  -184,  -184,  -184,  -184,  -184,
      -2,  -184,   135,   105,  -184,    37,  -184,  -184,    65,   140,
    -184,  -184,   133,   136,   147,  -184,  -184,  -184,  -184,   126,
    -184,  -184,    17,  -184,    17,  -184
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     2,     7,     0,     1,     0,     0,    11,
       4,     9,     3,     0,    19,     0,     0,     0,    10,    23,
      28,     9,     6,     0,    20,    18,    31,    40,     8,    13,
      14,    15,     0,    26,    29,    27,     0,     0,     0,    40,
      40,    17,     0,     0,    34,    48,    41,    65,     5,    39,
      38,     0,    12,    24,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    26,    21,    32,     0,     0,     0,     7,
      47,    67,    60,     0,     0,     0,    64,    61,    62,    63,
       0,    25,     0,    34,    36,    35,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,    17,    23,
      33,     0,    30,     7,    59,     0,    42,     0,     0,    98,
      99,   100,   101,   102,     0,    93,    94,    96,    95,    97,
       0,     0,     0,     0,     0,    69,    76,    82,    92,     0,
       0,     0,    16,    22,     0,     0,     0,     0,     0,    45,
       7,    68,    91,    89,    90,     0,   103,   106,   107,   108,
     105,   104,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    49,    57,     0,     0,    47,     0,    88,
     118,    70,    75,    73,    71,    79,    77,    78,    72,    74,
      80,    81,    86,    83,    84,    85,    87,    65,   116,   117,
       0,    66,     0,    59,    53,     7,    46,    44,   112,     0,
      37,    58,    56,     0,    65,   109,   110,   111,    65,     0,
      54,    51,   114,   113,   115,    55
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -184,  -184,  -184,  -184,   -69,  -184,   146,  -184,  -184,   137,
      71,  -184,    73,  -184,  -184,   138,  -184,  -184,  -184,  -184,
      87,  -184,  -184,    15,  -184,  -184,  -184,    -9,  -184,  -184,
    -184,   -48,  -184,  -184,  -184,   -27,  -184,  -184,  -183,  -184,
    -184,  -184,  -113,   -59,   -54,  -117,  -184,  -184,  -184,   -36,
    -184,  -184,  -184,  -184,  -184,  -184
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,     4,     8,     9,    16,    14,    18,    32,
      52,    20,    25,    33,    82,    43,    63,    27,    35,    44,
      56,    83,    87,    38,    39,    59,    60,    93,    40,    57,
      58,    90,   104,   212,   220,   137,   203,    48,    61,    76,
      77,    94,   124,   125,   126,   127,   128,   114,   152,    78,
     215,   216,   217,    79,   200,   129
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      91,   141,   115,   142,   143,   144,    36,    37,   164,    84,
     145,    85,   198,   199,   208,   165,   166,   170,    86,   157,
       5,    71,     6,   158,   159,   116,   117,   118,   119,   167,
     168,   222,     1,    73,   135,   224,    74,   162,   163,   180,
     120,    75,     7,   121,   122,   153,    11,   192,   193,   194,
     195,   196,    10,   123,    49,    50,   154,   155,   156,    12,
     157,   146,    13,    15,   158,   159,    29,    30,    31,    71,
      71,   178,   147,   148,   149,    72,   160,   161,   162,   163,
      17,    73,    73,   214,    74,    74,    19,   209,    21,    75,
      75,    22,   150,   151,   181,   182,   183,   184,    23,    24,
      26,   188,   189,   185,   186,   187,    34,    41,   190,   191,
     109,   110,   111,   112,   113,    42,    45,    46,    47,    51,
      53,    55,    54,   -50,   -43,    62,    65,    66,    64,    67,
      89,    69,    88,    92,    68,    96,   213,    70,    95,    98,
      97,    99,   102,   101,   103,   134,   130,   131,   174,   139,
     106,   197,   176,   202,   105,   107,   136,   140,   138,   204,
     108,   173,   218,    73,   169,   171,   210,    28,   206,   132,
     100,   225,   133,   172,   177,   175,   211,   201,   223,     0,
     205,   179,   207,   219,     0,     0,   221,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    80,
       0,    81
};

static const yytype_int16 yycheck[] =
{
      69,   114,     4,   120,   121,   122,    10,    11,    40,     4,
     123,     6,    20,    21,   197,    47,    48,   130,    13,    41,
       4,     4,     0,    45,    46,    27,    28,    29,    30,    61,
      62,   214,     3,    16,   103,   218,    19,    59,    60,   152,
      42,    24,     5,    45,    46,    26,     4,   164,   165,   166,
     167,   168,    50,    55,    39,    40,    37,    38,    39,    49,
      41,    26,    15,    51,    45,    46,    27,    28,    29,     4,
       4,   140,    37,    38,    39,     9,    57,    58,    59,    60,
       4,    16,    16,    18,    19,    19,     6,   200,     4,    24,
      24,    50,    57,    58,   153,   154,   155,   156,    26,     4,
       7,   160,   161,   157,   158,   159,     4,    50,   162,   163,
      32,    33,    34,    35,    36,    51,     4,     4,     8,     4,
       4,    51,    26,    55,    55,    26,     4,    52,    31,    52,
       4,    50,    31,     4,    55,     4,   205,    55,    55,    50,
      55,    50,    50,    53,    50,    25,    32,    30,     4,    31,
      50,    17,    31,    14,    56,    52,    51,    50,    52,    31,
      56,    50,    22,    16,    56,    56,    31,    21,   177,    98,
      83,   219,    99,    54,    50,    52,   203,    50,   214,    -1,
      50,    56,    50,    50,    -1,    -1,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      -1,    63
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    64,    65,    66,     4,     0,     5,    67,    68,
      50,     4,    49,    15,    70,    51,    69,     4,    71,     6,
      74,     4,    50,    26,     4,    75,     7,    80,    69,    27,
      28,    29,    72,    76,     4,    81,    10,    11,    86,    87,
      91,    50,    51,    78,    82,     4,     4,     8,   100,    86,
      86,     4,    73,     4,    26,    51,    83,    92,    93,    88,
      89,   101,    26,    79,    31,     4,    52,    52,    55,    50,
      55,     4,     9,    16,    19,    24,   102,   103,   112,   116,
      72,    78,    77,    84,     4,     6,    13,    85,    31,     4,
      94,    67,     4,    90,   104,    55,     4,    55,    50,    50,
      83,    53,    50,    50,    95,    56,    50,    52,    56,    32,
      33,    34,    35,    36,   110,     4,    27,    28,    29,    30,
      42,    45,    46,    55,   105,   106,   107,   108,   109,   118,
      32,    30,    73,    75,    25,    67,    51,    98,    52,    31,
      50,   105,   108,   108,   108,   105,    26,    37,    38,    39,
      57,    58,   111,    26,    37,    38,    39,    41,    45,    46,
      57,    58,    59,    60,    40,    47,    48,    61,    62,    56,
     105,    56,    54,    50,     4,    52,    31,    50,    67,    56,
     105,   106,   106,   106,   106,   107,   107,   107,   106,   106,
     107,   107,   108,   108,   108,   108,   108,    17,    20,    21,
     117,    50,    14,    99,    31,    50,    90,    50,   101,   105,
      31,    98,    96,    67,    18,   113,   114,   115,    22,    50,
      97,    50,   101,   112,   101,    94
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    63,    64,    65,    66,    67,    68,    68,    69,    69,
      70,    70,    71,    72,    72,    72,    73,    73,    74,    74,
      76,    77,    75,    75,    79,    78,    78,    80,    80,    82,
      81,    81,    84,    83,    83,    85,    85,    85,    86,    86,
      86,    88,    87,    89,    87,    90,    90,    90,    92,    91,
      93,    91,    95,    96,    94,    97,    97,    99,    98,    98,
     100,   101,   101,   101,   101,   101,   102,   104,   103,   105,
     105,   105,   105,   105,   105,   105,   106,   106,   106,   106,
     106,   106,   107,   107,   107,   107,   107,   107,   108,   108,
     108,   108,   108,   108,   109,   109,   109,   109,   110,   110,
     110,   110,   110,   111,   111,   111,   111,   111,   111,   112,
     113,   113,   113,   114,   115,   116,   117,   117,   118
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     3,     3,     6,     4,     0,     3,     0,
       2,     0,     5,     1,     1,     1,     5,     0,     2,     0,
       0,     0,     8,     0,     0,     4,     0,     2,     0,     0,
       6,     0,     0,     4,     0,     1,     1,     6,     2,     2,
       0,     0,     6,     0,     9,     3,     5,     0,     0,     8,
       0,    11,     0,     0,     7,     2,     0,     0,     4,     0,
       3,     2,     2,     2,     2,     0,     5,     0,     4,     1,
       3,     3,     3,     3,     3,     3,     1,     3,     3,     3,
       3,     3,     1,     3,     3,     3,     3,     3,     3,     2,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     7,
       1,     1,     0,     2,     2,     8,     1,     1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

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

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



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

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
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
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
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
            else
              goto append;

          append:
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

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
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
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 6:
#line 231 "pascal.y"
                                          {
                // uses_identifier_top++;
		// uses_identifier_stack[uses_identifier_top] = strdup(yylval.s.str);
                // might need to edit so I'm leaving
                // ast may come here (line 195)
        }
#line 1694 "y.tab.c"
    break;

  case 8:
#line 241 "pascal.y"
                                    {
                // uses_identifier_top++;
		// uses_identifier_stack[uses_identifier_top] = strdup(yylval.s.str);
                // might need to edit so I'm leaving this one as well 
        }
#line 1704 "y.tab.c"
    break;

  case 20:
#line 276 "pascal.y"
        {
            type_identifier_top++;
			type_identifier_stack[type_identifier_top] = strdup(yylval.str);
        }
#line 1713 "y.tab.c"
    break;

  case 21:
#line 281 "pascal.y"
        {
            for(int i = 0; i <= type_identifier_top; i++)
			{
				struct type_table *s = NULL;
				HASH_FIND_STR(TYPE_TABLE, type_identifier_stack[i], s);
				if(!s)
				{
					s = malloc(sizeof(struct type_table));
					strcpy(s->user_defined_name, type_identifier_stack[i]);
					strcpy(s->actual_type_name, yylval.type);
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
#line 1738 "y.tab.c"
    break;

  case 24:
#line 307 "pascal.y"
        {
            type_identifier_top++;
			type_identifier_stack[type_identifier_top] = strdup(yylval.str);
        }
#line 1747 "y.tab.c"
    break;

  case 29:
#line 323 "pascal.y"
        {
            var_name_stack_top++;
			var_name_stack[var_name_stack_top] = strdup(yylval.str);
        }
#line 1756 "y.tab.c"
    break;

  case 32:
#line 333 "pascal.y"
        {
            type_identifier_top++;
			type_identifier_stack[type_identifier_top] = strdup(yylval.str);
        }
#line 1765 "y.tab.c"
    break;

  case 35:
#line 343 "pascal.y"
        {
			int result = dump_stack_in_symbol_table(yylval.type, yylloc.first_line, yylloc.first_column);
                if(!result){
                        yyerror(" abort, Variable already declared.");
                        exit(1);

                }

		}
#line 1779 "y.tab.c"
    break;

  case 36:
#line 354 "pascal.y"
        {
            struct type_table *t = NULL;
			HASH_FIND_STR(TYPE_TABLE,yylval.str,t);

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
				printf("Alert : Type %s is not defined.",yylval.str);
				YYABORT;
			}
        }
#line 1802 "y.tab.c"
    break;

  case 37:
#line 373 "pascal.y"
                {
			//printf("Hit the type part of line %s\n", yylval.type);
			int result = dump_stack_in_symbol_table("array", yylloc.first_line, yylloc.first_column);
			if(!result){
				//printf("DumpBck in Variable: %d\n",result);
				yyerror("Abort: Variable already declared.");
				exit(1);
			}
		}
#line 1816 "y.tab.c"
    break;

  case 41:
#line 392 "pascal.y"
                {
			curr_scope_level = strdup(yylval.str);
			printf("Entering the Procedure %s\n", curr_scope_level);
		}
#line 1825 "y.tab.c"
    break;

  case 43:
#line 398 "pascal.y"
                {
			curr_scope_level = strdup(yylval.str);
			printf("Entering the Procedure %s\n", curr_scope_level);
		}
#line 1834 "y.tab.c"
    break;

  case 48:
#line 413 "pascal.y"
                {
			curr_scope_level = strdup(yylval.str);
			printf("Entering the Function %s\n", curr_scope_level);
		}
#line 1843 "y.tab.c"
    break;

  case 49:
#line 418 "pascal.y"
                {
			strcpy(curr_scope_level,"global");

		}
#line 1852 "y.tab.c"
    break;

  case 50:
#line 423 "pascal.y"
                {
			curr_scope_level = strdup(yylval.str);
		}
#line 1860 "y.tab.c"
    break;

  case 51:
#line 427 "pascal.y"
                {
			char s[10] = "global";
			curr_scope_level = strdup(s);
		}
#line 1869 "y.tab.c"
    break;

  case 52:
#line 435 "pascal.y"
                {
			var_name_stack_top++;
			var_name_stack[var_name_stack_top] = strdup(yylval.str);
		}
#line 1878 "y.tab.c"
    break;

  case 53:
#line 440 "pascal.y"
                {
		int result = dump_stack_in_symbol_table(yylval.type, yylloc.first_line, yylloc.first_column);
		if(!result){
				yyerror("Abort: Variable already declared.");
				exit(1);
			}
		}
#line 1890 "y.tab.c"
    break;

  case 57:
#line 457 "pascal.y"
                {
			var_name_stack_top++;
			var_name_stack[var_name_stack_top] = strdup(yylval.str);
		}
#line 1899 "y.tab.c"
    break;

  case 67:
#line 483 "pascal.y"
        {
			if(!check_valid_identifier(yylval.str)){
				char error[1000];
				//printf("Scope Level : %s ",curr_scope_level);
				sprintf(error,"Abort: Variable %s is not declared.",yylval.str);
				yyerror(error);
				exit(1);
			}
			else
			{
				assignment_name_stack_top++;
				assignment_name_stack[assignment_name_stack_top] = strdup(yylval.str);
			}
        }
#line 1918 "y.tab.c"
    break;

  case 75:
#line 508 "pascal.y"
        {
                // not sure what this is so I left      
            printf("%d and %d and %s\n",(yyvsp[-2].intval),(yyvsp[0].intval),(yyvsp[-1].str));
			(yyval.intval) = solution((yyvsp[-2].intval),(yyvsp[0].intval),(yyvsp[-1].str));
			
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
				temp->var_value.int_value = (yyval.intval);
				HASH_REPLACE_STR(SYMBOL_TABLE, var_name, temp, r);  /* var_name: name of key field */
			}
        }
#line 1948 "y.tab.c"
    break;

  case 81:
#line 542 "pascal.y"
                {
                // not sure what this is so I left      
            printf("%d and %d and %s\n",(yyvsp[-2].intval),(yyvsp[0].intval),(yyvsp[-1].str));
			(yyval.intval) = solution((yyvsp[-2].intval),(yyvsp[0].intval),(yyvsp[-1].str));
			
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
				temp->var_value.int_value = (yyval.intval);
				HASH_REPLACE_STR(SYMBOL_TABLE, var_name, temp, r);  /* var_name: name of key field */
			}
        }
#line 1978 "y.tab.c"
    break;

  case 87:
#line 576 "pascal.y"
                {
                // not sure what this is so I left      
            printf("%d and %d and %s\n",(yyvsp[-2].intval),(yyvsp[0].intval),(yyvsp[-1].str));
			(yyval.intval) = solution((yyvsp[-2].intval),(yyvsp[0].intval),(yyvsp[-1].str));
			
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
				temp->var_value.int_value = (yyval.intval);
				HASH_REPLACE_STR(SYMBOL_TABLE, var_name, temp, r);  /* var_name: name of key field */
			}
        }
#line 2008 "y.tab.c"
    break;

  case 93:
#line 610 "pascal.y"
                {
			if(check_valid_identifier(yyval.str)) {
				union data variable_value = get_identifier_data(yylval.str);
				(yyval.intval) = variable_value.int_value;
			}
        }
#line 2019 "y.tab.c"
    break;

  case 94:
#line 619 "pascal.y"
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
				temp->var_value.int_value = yylval.intval;
				HASH_REPLACE_STR( SYMBOL_TABLE, var_name, temp,r );  /* var_name: name of key field */
			}
        }
#line 2045 "y.tab.c"
    break;

  case 95:
#line 641 "pascal.y"
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
				temp->var_value.float_value = yylval.floatval;
				HASH_REPLACE_STR( SYMBOL_TABLE, var_name, temp,r );  /* var_name: name of key field */
			}
        }
#line 2071 "y.tab.c"
    break;

  case 96:
#line 663 "pascal.y"
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
				temp->var_value.int_value = yylval.intval;
				HASH_REPLACE_STR( SYMBOL_TABLE, var_name, temp,r );  /* var_name: name of key field */
			}
        }
#line 2097 "y.tab.c"
    break;

  case 97:
#line 685 "pascal.y"
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
				strcpy(temp->var_value.string_value,yylval.str);
				HASH_REPLACE_STR( SYMBOL_TABLE, var_name, temp,r );  /* var_name: name of key field */
			}
        }
#line 2123 "y.tab.c"
    break;


#line 2127 "y.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

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
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
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
  return yyresult;
}
#line 755 "pascal.y"

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
