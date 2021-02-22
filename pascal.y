%{
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
        
%}
%locations 

%union {
	struct parse_node
	{
		
		char *str;
		char *type;
		int intval;
		float floatval;
		char* stringval;
		// struct ast_node * ast;
	}s;
}

%token T_PROGRAM;
%token <str> T_IDENTIFIER;
%token T_USES;
%token T_TYPE;
%token T_VAR;
%token T_BEGIN;
%token T_END;
%token T_FUNCTION;
%token T_PROCEDURE;
%token T_ARRAY;
%token T_OF;
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

%token T_INDEXTYPE;

%token T_SINGLEEQ;

%token <intval> T_INTVAL;
%token <intval> T_BOOLVAL;
%token <floatval> T_FLOATVAL;
%token <str> T_STRINGVAL;
%token <type> T_DATATYPE;

%token T_ASOP;
%token T_AS_PE;
%token T_AS_SE;
%token T_AS_MULE;
%token T_AS_DIVE;

%token T_GE;
%token T_LE;
%token T_NE;

%token T_BOOL_AND;
%token T_BOOL_OR;
%token T_BOOL_NOT;
%token T_BIT_LEFT_SHIFT;
%token T_BIT_RIGHT_SHIFT;

%right T_ASOP
%left '+' '-'
%left '*' '/'
%%

// Pascal Program structure
// program _name_of_the_program_
// uses _packages_program_uses_
// const _global_const_declaration_block_
// var _global_variable_declaration_block_
// begin _main_program_starts_
// code 
// end. _main_program_ends_


startProg :
		program
		;

program :
        programHeading block '.'
        ;

programHeading :
        T_PROGRAM T_IDENTIFIER ';'
        ;

// just | followed by ; is for lambda

block   :
        uses_block const_block type_block variable_block function_and_procedure_block execution_block
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
        T_IDENTIFIER T_SINGLEEQ constant{ 			
		//printf("%s %s %d %.5f %s %s\n",$<s.str>1,$<s.type>1,$<s.intval>1,$<s.floatval>3,$<s.stringval>1,$<s.type>3); 
		// printf("yylval: %s\n",yylval.s.str);
		struct symbol_table *s = NULL;
		HASH_FIND_STR(SYMBOL_TABLE,$<s.str>1, s);
		if(!s){
			s = malloc(sizeof(struct symbol_table));
			strcpy(s->var_name,yylval.s.str);
			strcpy(s->type,yylval.s.type);
			s->scope_level = strdup("const");
			s->line_no = yylloc.first_line;
			s->col_no = yylloc.first_column;
			if(yylval.s.intval!=0){
				s->var_value.int_value = $<s.intval>3;
			}
			if(yylval.s.floatval!=0){
				printf("%f", $<s.floatval>3);
				s->var_value.float_value = $<s.floatval>3;
			}

			HASH_ADD_STR(SYMBOL_TABLE, var_name, s);
			//printf("yayy\n");
		}else{
			//printf("ono\n");
			}
		// printf("%s %s %d %f \n",$<s.str>2,$<s.type>2,$<s.intval>2,$<s.floatval>2); 
		// printf("%s %s %d %f \n",$<s.str>3,$<s.type>3,$<s.intval>3,$<s.floatval>3); 
		
		}';' more_const_definition
        ;

constant :
        T_INTVAL
        | T_BOOLVAL
        | T_FLOATVAL
        | T_STRINGVAL
        ;

more_const_definition :
        T_IDENTIFIER T_SINGLEEQ constant{
			// var_name_stack_top++;
			// var_name_stack[var_name_stack_top] = strdup(yylval.s.str);
		}
		';' more_const_definition
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
			type_identifier_stack[type_identifier_top] = strdup(yylval.s.str);
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
        ';' type_definition
        |
        ;

more_type_identifiers :
        ',' T_IDENTIFIER 
        {
            type_identifier_top++;
			type_identifier_stack[type_identifier_top] = strdup(yylval.s.str);
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
			var_name_stack[var_name_stack_top] = strdup(yylval.s.str);
        }
        more_decl_stmt ':' data_type ';'
        |
        ;

more_decl_stmt :
        ',' T_IDENTIFIER
        {
            var_name_stack_top++;
			var_name_stack[var_name_stack_top] = strdup(yylval.s.str);
        } 
        more_decl_stmt
        |
        ;

data_type :
        T_DATATYPE
        {
			int result = dump_stack_in_symbol_table(yylval.s.type, yylloc.first_line, yylloc.first_column);
			if(!result){
					yyerror(" abort, Variable already declared.");
					exit(1);

			}

		}
        |
        T_IDENTIFIER
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
               
function_and_procedure_block:
		function_block function_and_procedure_block 
		| procedure_block function_and_procedure_block 
		| 
		;

procedure_block:
		T_PROCEDURE T_IDENTIFIER
		{
			curr_scope_level = strdup(yylval.s.str);
			printf("Entering the Procedure %s\n", curr_scope_level);
		}
		';'  block ';'
		| T_PROCEDURE T_IDENTIFIER 
		{
			curr_scope_level = strdup(yylval.s.str);
			printf("Entering the Procedure %s\n", curr_scope_level);
		}
		'(' param_list ')' ';'  block ';'
		;

param_list:
		T_IDENTIFIER ':' T_DATATYPE 
		| T_IDENTIFIER ':' T_DATATYPE ';' param_list
		| 
		;

function_block:
		T_FUNCTION T_IDENTIFIER
		{
			curr_scope_level = strdup(yylval.s.str);
			printf("Entering the Function %s\n", curr_scope_level);
		}
		':' T_DATATYPE ';'  block ';' 
		{
			strcpy(curr_scope_level,"global");

		}
		| T_FUNCTION T_IDENTIFIER 
		{
			curr_scope_level = strdup(yylval.s.str);
		}
		'(' function_param_list ')' ':' T_DATATYPE ';'  block ';' 
		{
			char s[10] = "global";
			curr_scope_level = strdup(s);
		}
		;

function_param_list:
		T_IDENTIFIER 
		{
			var_name_stack_top++;
			var_name_stack[var_name_stack_top] = strdup(yylval.s.str);
		}
		more_func_identifiers ':' T_DATATYPE 
		{
		int result = dump_stack_in_symbol_table(yylval.s.type, yylloc.first_line, yylloc.first_column);
		if(!result){
				yyerror("Abort: Variable already declared.");
				exit(1);
			}
		}
		function_param_continue
		;

function_param_continue :
		';' function_param_list
		|
		;

more_func_identifiers:
		',' T_IDENTIFIER
		{
			var_name_stack_top++;
			var_name_stack[var_name_stack_top] = strdup(yylval.s.str);
		}
		more_func_identifiers 
		| 
		;

execution_block :
        T_BEGIN statementList T_END
        ;

statementList :
		statements
		| statements ';' statementList
		;

statements :
		execution_block
		| assignment_statements 
        | if_statement
        | fordo_statement
        | print_statements
        |
        ;
	
print_statements:
		T_WRITELN '(' T_STRINGVAL ')' ';'
;

assignment_statements :
        T_IDENTIFIER 
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
        assignment_operators expression
        ;

expression :
        simpleExpression
        | simpleExpression T_SINGLEEQ simpleExpression
		| simpleExpression T_NE simpleExpression
		| simpleExpression '<' simpleExpression
		| simpleExpression T_LE simpleExpression
		| simpleExpression '>' simpleExpression
		| simpleExpression T_GE simpleExpression
        {
                // not sure what this is so I left      
            printf("%d and %d and %s\n",$<s.intval>1,$<s.intval>3,$<s.str>2);
			$<s.intval>$ = solution($<s.intval>1,$<s.intval>3,$<s.str>2);
			
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
				temp->var_value.int_value = $<s.intval>$;
				HASH_REPLACE_STR(SYMBOL_TABLE, var_name, temp, r);  /* var_name: name of key field */
			}
        }
        ;

simpleExpression :
		term
		| simpleExpression '+' term
		| simpleExpression '-' term
		| simpleExpression T_BOOL_OR term
		| simpleExpression '|' term
		| simpleExpression '!' term
		{
                // not sure what this is so I left      
            printf("%d and %d and %s\n",$<s.intval>1,$<s.intval>3,$<s.str>2);
			$<s.intval>$ = solution($<s.intval>1,$<s.intval>3,$<s.str>2);
			
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
				temp->var_value.int_value = $<s.intval>$;
				HASH_REPLACE_STR(SYMBOL_TABLE, var_name, temp, r);  /* var_name: name of key field */
			}
        }
		;

term :
		factor 
		| term '*' factor
		| term '/' factor
		| term '%' factor
		| term T_BOOL_AND factor
		| term '&' factor
		{
                // not sure what this is so I left      
            printf("%d and %d and %s\n",$<s.intval>1,$<s.intval>3,$<s.str>2);
			$<s.intval>$ = solution($<s.intval>1,$<s.intval>3,$<s.str>2);
			
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
				temp->var_value.int_value = $<s.intval>$;
				HASH_REPLACE_STR(SYMBOL_TABLE, var_name, temp, r);  /* var_name: name of key field */
			}
        }
		;

factor :
		'(' expression ')'
		| '+' factor
		| '-' factor
		| T_BOOL_NOT factor
		| value
		| T_IDENTIFIER
		{
			if(check_valid_identifier(yyval.s.str)) {
				union data variable_value = get_identifier_data(yylval.s.str);
				$<s.intval>$ = variable_value.int_value;
			}
        }

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
				temp->var_value.int_value = yylval.s.intval;
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
				temp->var_value.float_value = yylval.s.floatval;
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
				temp->var_value.int_value = yylval.s.intval;
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
				strcpy(temp->var_value.string_value,yylval.s.str);
				HASH_REPLACE_STR( SYMBOL_TABLE, var_name, temp,r );  /* var_name: name of key field */
			}
        }
        ;

assignment_operators :
        T_ASOP
        | T_AS_PE
        | T_AS_SE
        | T_AS_MULE
        | T_AS_DIVE
        ;

relational_operators :
        T_SINGLEEQ
        | '>'
        | '<'
        | T_GE
        | T_LE
        | T_NE

if_statement :
        T_IF '(' boolean_expression ')' T_THEN statements T_ELSE statements
		| T_IF '(' boolean_expression ')' T_THEN statements
        ;

fordo_statement :
        T_FOR T_IDENTIFIER T_ASOP expression to_or_downto expression T_DO statements
        ;

to_or_downto :
        T_TO
        | T_DOWNTO
        ;

boolean_expression :
		expression relational_operators expression
;

%%
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
				else if(strcmp(s->type,"float")==0){
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
