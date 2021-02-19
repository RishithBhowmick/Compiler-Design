%{
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
    
    union data {    // since value is not of a single type. Union is the best structure.
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

	char *type_identifier_stack[10];
	int type_identifier_top = -1;

	struct type_table{
		char user_defined_name[31];
		char actual_type_name[31];
		UT_hash_handle hh;
	};
	struct type_table *TYPE_TABLE = NULL;

        int dump_stack_in_symbol_table(char *type, int line_no, int col_no) {
		for(int i = 0; i <= var_name_stack_top; i++)
		{
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

        int check_valid_identifier(char* var_name){
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

        union data get_identifier_data(char *var_name){
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
		if(strcmp(operator,"+")==0)
		{
			result = a+b;

		}
		if(strcmp(operator,"*")==0)
		{
			result = a*b;

		}
		if(strcmp(operator,"/")==0)
		{
			result = a/b;

		}
		if(strcmp(operator,"-")==0)
		{
			result = a-b;

		}
		if(strcmp(operator,"%")==0)
		{
			result = a%b;

		}
		return result;
	}


        
%}
%token T_PROGRAM;
%token T_IDENTIFIER;
%token T_TYPE;
%token T_VAR;
%token T_BEGIN;
%token T_END;
%token T_FUNCTION;
%token T_PROCEDURE;
%token T_AND;
%token T_ARRAY;
%token T_CONST;
%token T_IF;
%token T_THEN;
%token T_ELSE;
%token T_FOR;
%token T_TO;
%token T_DOWNTO;
%token T_DO;
%token T_WRITE;
%token T_WRITELN;
%%

// Pascal Program structure
// program _name_of_the_program_
// uses _packages_program_uses_
// const _global_const_declaration_block_
// var _global_variable_declaration_block_
// begin _main_program_starts_
// code 
// end. _main_program_ends_

Program :
        ProgramHeading block
        ;

ProgramHeading :
        T_PROGRAM T_IDENTIFIER ';'
        ;

// just | followed by ; is for lambda

block   :
        uses_block const_block type_block variable_block execution_block "."
        ;

uses_block :
        T_USES T_IDENTIFIER other_libs ';'{
                // uses_identifier_top++;
		// uses_identifier_stack[uses_identifier_top] = strdup(yylval.s.str);
                // might need to edit so I'm leaving
                // ast may come here (line 195)
        }
        |
        ;

other_libs :
        ',' T_IDENTIFIER other_libs {
                // uses_identifier_top++;
		// uses_identifier_stack[uses_identifier_top] = strdup(yylval.s.str);
                // might need to edit so I'm leaving this one as well 
        }
        |
        ;

const_block :
        T_CONST const_definition
        |
        ;

const_definition :
        T_IDENTIFIER T_SINGLEEQ constant ';' more_const_definition
        ;

constant :
        T_INTVAL
        | T_BOOLVAL
        | T_FLOATVAL
        ;

more_const_definition :
        T_IDENTIFIER T_SINGLEEQ constant ';' more_const_definition
        |
        ;

type_block :
        T_TYPE type_definition
        |
        ;

type_definition :
        T_IDENTIFIER 
        {
                type_identifier_top++;
		type_identifier_stack[type_identifier_top] = strdup(yylval.str);
        }
        more_type_identifiers T_SINGLEEQ T_DATATYPE 
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
				HASH_ADD_STR( TYPE_TABLE, user_defined_name, s );  /* var_name: name of key field */
			}
			else
			{
				YYABORT;
			}
			type_identifier_stack[i] = NULL;
		}
		type_identifier_top = -1;
        }
        ';' type_definition
        |
        ;

more_type_identifiers :
        ',' T_IDENTIFIER 
        {
                type_identifier_top++;
		type_identifier_stack[type_identifier_top] = strdup(yylval.str);
        }
        more_type_identifiers
        |
        ;

variable_block :
        T_VAR decl_stmts
        |
        ;

// here symbol table stuff will come
decl_stmts :
        T_IDENTIFIER
        {
                var_name_stack_top++;
		var_name_stack[var_name_stack_top] = strdup(yylval.str);
        }
        more_decl_stmt ":" data_type ";"
        |
        ;

more_decl_stmt :
        "," T_IDENTIFIER
        {
                type_identifier_top++;
		type_identifier_stack[type_identifier_top] = strdup(yylval.str);
        } 
        more_decl_stmt
        |
        ;
data_type :
        T_TYPE
        {
		int result = dump_stack_in_symbol_table(yylval.type, yylloc.first_line, yylloc.first_column);
                if(!result){
                        yyerror(" abort, Variable already declared.");
                        exit(1);

                }

	}
        |
        T_IDENTIFIER
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
        | T_ARRAY '[' T_INDEXTYPE ']' T_OF T_DATATYPE
	{
		//printf("Hit the type part of line %s\n", yylval.type);
		int result = dump_stack_in_symbol_table("array", yylloc.first_line, yylloc.first_column);
		if(!result){
			//printf("DumpBck in Variable: %d\n",result);
			yyerror("Abort: Variable already declared.");
			exit(1);
		}

	}
        ;  
               


execution_block :
        T_BEGIN execution_body T_END
        ;

execution_body :
        assignment_statements execution_body
        | if_statement execution_body
        | fordo_statement execution_body
        | print_statements execution_body
        |
        ;

assignment_statements :
        T_IDENTIFIER 
        {
                //printf("Variable Being Checked : %s ",yylval.str);
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
        assignment_operators expression
        ;

expression :
        T_IDENTIFIER
        {
                if(check_valid_identifier(yyval.str)){
			union data variable_value = get_identifier_data(yylval.str);
			$<intval>$ = variable_value.int_value;
		}
        }
        | value
        | "(" expression ")"
        | expression operator expression
        {
                // not sure what this is so I left      
                printf("%d and %d and %s\n",$<intval>1,$<intval>3,$<str>2);
		$<intval>$ = solution($<intval>1,$<intval>3,$<str>2);
		
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
			temp->var_value.int_value = $<intval>$;
			HASH_REPLACE_STR( SYMBOL_TABLE, var_name, temp,r );  /* var_name: name of key field */
		}
        }
        ;

value :
        T_INTVAL
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
        | T_FLOATVAL
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
        | T_BOOLVAL
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
        | T_STRINGVAL
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
        ;

operator :
        arithemtic_operators
        | relational_operators
        | boolean_operators
        | bitwise_operators
        ;

assignment_operators :
        T_ASOP
        | T_AS_PE
        | T_AS_SE
        | T_AS_MULE
        | T_AS_DIVE
        ;

arithemtic_operators :
        '+'
        | '-'
        | '*'
        | '/'
        | '%'
        ;

relational_operators :
        T_SINGLEEQ
        | '>'
        | '<'
        | T_GE
        | T_LE
        | T_NE
        ;

boolean_operators :
        T_BOOL_AND
        | T_BOOL_OR
        | T_BOOL_NOT
        ;

bitwise_operators :
        '|'
        | '&'
        | '!'
        | '~'
        | T_BIT_LEFT_SHIFT
        | T_BIT_RIGHT_SHIFT
        ;

if_statement :
        T_IF '(' boolean_statement ')' T_THEN execution_body
        | T_IF '(' boolean_statement ')' T_THEN execution_body T_ELSE execution_body
        ;

fordo_statement :
        T_FOR T_IDENTIFIER T_ASOP expression to_or_downto expression T_DO execution_body
        ;

to_or_downto :
        T_TO
        | T_DOWNTO
        ;

boolean_expression :
	expression relational_operators expression
;

expression :
	T_IDENTIFIER
	{
		if(check_valid_identifier(yyval.str)){
			union data variable_value = get_identifier_data(yylval.str);
			$<intval>$ = variable_value.int_value;
		}
	} 
	| value 
	| '(' expression ')' 
	| expression operator expression 
	{
		printf("%d and %d and %s\n",$<intval>1,$<intval>3,$<str>2);
		$<intval>$ = solution($<intval>1,$<intval>3,$<str>2);
		
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
			temp->var_value.int_value = $<intval>$;
			HASH_REPLACE_STR( SYMBOL_TABLE, var_name, temp,r );  /* var_name: name of key field */
		}
	}
;
%%
int yyparse() {
        yylex();
}
int yyerror() {
        printf("Invalid Syntax: %d: %d\n", yylloc.first_line, yylloc.first_column);
        printf("Compilation failed.\n");
        successful = 0;
        return 0;
}

int main() {
        yyparse();
}