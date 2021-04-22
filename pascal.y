%{
    #include<stdio.h>
    #include<stdlib.h>
    #include<time.h>
	#include<string.h>
    #include "uthash/src/uthash.h"
    #include "sym_tab.h" 
	#include "ast.h"
	#include "icg.h"
    #define YYPARSE_PARAM scanner
    #define YYLEX_PARAM   scanner
	#define COUNT 5
	
    int yylex();
    int yyerror();
    int successful=1;
	int is_rel_op = 0;
	int top = -1;
    extern FILE *yyin;
    extern FILE *yyout;
    double time_elapsed(struct timespec *start, struct timespec *end);
	char* get_type(char* op1);
	//void check_types(char* op1, char* op2);
	void check_t(char* t1, char* t2);
	void check_assign(char* t1, char* t2);
	void push();
	void push_symbol(char* symbol);
	void push_value(char* type);
	void for1();
	void for2();
	void for3();
	void for4();
	void if1();
	void if2();
	void if3();
	void codegen();
	void codegen_assign();
	void codegen_assigna();
	void call_function();
	void write_params(int n);
	int l = 0;
	int in_write = 0;
	FILE *fptr;
	char *var_name_stack[100];
	int var_name_stack_top = -1;
	char *assignment_name_stack[31];
	int assignment_name_stack_top = -1;
	char *type_stack[100];
	int type_stack_top = -1;
	int write_param_count;
	char *curr_scope_level = "global";
	//char** ops = ["+", "-", "*", "/"];
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
	
	#define MAX_TYPE 20

	struct type_node_info {
		char **new_types;
		int type_identifier_top;
		char *actual_type;
	};

	struct type_node_info type_node_stack[20];
	int type_node_stack_top = -1;
	struct const_table* CONST_TABLE = NULL;
	struct type_table *TYPE_TABLE = NULL;
	struct ast_node *tree;
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
				// printf("\nAlert : Inserting Variable '%s' in to the Symbol Table.\n", var_mang_name);
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
				fprintf(fptr,"%s %s\n",s->type,s->var_name);
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
		if(!s){
			strcpy(var_mang_name, var_name);
			strcat(var_mang_name, "$");
			strcat(var_mang_name, "const");
			
			HASH_FIND_STR(SYMBOL_TABLE, var_mang_name, s);
			if(!s){
				return 0;
			}
			else{
				return 1;
			}
			return 0;	
		}		
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
    //  node* construct_AST(node* left, node* right, char* token){
	// 	node* newnode = (node*)malloc(sizeof(node));
	// 	char* newstr = (char*)malloc(strlen(token)+1);
	// 	strcpy(newstr, token);
	// 	newnode->left = left;
	// 	newnode->right = right;
	// 	newnode->token = token;
	// 	return newnode; 
	// }

	// void disp(node* root, int space){
	// 	if(root == NULL){
	// 		return;
	// 	}
	// 	space += COUNT;
	// 	disp(root-> right, space);
	// 	printf("\n");
	// 	for(int i = COUNT; i < space; i++){
	// 		printf(" ");
	// 	}
	// 	printf("%s\n", root->token);
	// 	disp(root->left, space);
	// }

	// void DisplayTree(node* tree){
	// 	disp(tree, 0);
	// }
	int quadlen = 0;
	char st[100][100];
	quad q[100];	// declation of quadruples
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
		// struct ast_node* ast;
	}s;
}

%token T_PROGRAM;
%token <s.str> T_IDENTIFIER;
//%type <s.intval> value;
//%type <s.floatval> factor;
//%type <s.intval> term;
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
%token T_PROCALL;
%token T_INDEXTYPE;
%token T_SINGLEEQ;

%token <s.intval> T_INTVAL;
%token <s.intval> T_BOOLVAL;
%token <s.floatval> T_FLOATVAL;
%token <s.str> T_STRINGVAL;
%token <s.type> T_DATATYPE;

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
%token T_FORWARD;
%token T_DIV;
%token T_MOD;
%token T_DUBDOT;

%right T_ASOP
%left '+' '-'
%left '*' '/'


%nonassoc T_IFX
%nonassoc T_ELSE

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
		{ 
			//$<s.ast>$ = new_ast_root_node($<s.ast>1,$<s.ast>2);
		}
		| error '.'
        ;

programHeading :
        T_PROGRAM T_IDENTIFIER ';'  {
			printf("%s\n",$<s.str>2);
			char* filename = strcat($<s.str>2,".tac");
			fptr = fopen(filename,"w");
			}
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
        | error ';'
		|
        ;

other_libs :
        ',' T_IDENTIFIER other_libs
        |
        ;

const_block :
        T_CONST const_definition
        |
        ;

const_definition :
        T_IDENTIFIER T_SINGLEEQ constant{
		
		struct symbol_table *s = NULL;
		HASH_FIND_STR(SYMBOL_TABLE,$<s.str>1, s);
		if(!s){
			s = malloc(sizeof(struct symbol_table));
			strcpy(s->type,yylval.s.type);
			s->scope_level = strdup("const");
			char var_mang_name[31];
			strcpy(var_mang_name, yylval.s.str);
			strcat(var_mang_name, "$");
			strcat(var_mang_name, s->scope_level);
			strcpy(s->var_name,var_mang_name);
			// printf("\nAlert : Inserting Variable '%s' in to the Symbol Table.\n", var_mang_name);
			s->line_no = yylloc.first_line;
			s->col_no = yylloc.first_column;
			if(yylval.s.intval!=0){
				s->var_value.int_value = $<s.intval>3;
				fprintf(fptr,"%s %d\n",yylval.s.str,yylval.s.intval);
				// printf("%s = %d",s->var_name,s->var_value.intval);
			}
			if(yylval.s.floatval!=0){				
				s->var_value.float_value = $<s.floatval>3;
				fprintf(fptr,"%s %f\n",yylval.s.str,yylval.s.floatval);
				//printf("%s = %s",yylval.s.str,yylval.s.type);
			}
			if (yylval.s.stringval != NULL){
				fprintf(fptr,"%s %s\n",yylval.s.str,yylval.s.stringval);
			}

			HASH_ADD_STR(SYMBOL_TABLE, var_name, s);
			//printf("yayy\n");
		}
		// printf("%s %s %d %f \n",$<s.str>2,$<s.type>2,$<s.intval>2,$<s.floatval>2); 
		// printf("%s %s %d %f \n",$<s.str>3,$<s.type>3,$<s.intval>3,$<s.floatval>3); 
		//
		$<s.type>1 = $<s.type>3;
		//printf("C%s %s\n",$<s.type>1, $<s.type>3);
		printf("%s = %s\n",$<s.str>1,st[top]);
		q[quadlen].op = (char*)malloc(sizeof(char));
    	q[quadlen].arg1 = (char*)malloc(sizeof(char)*10);
    	q[quadlen].arg2 = NULL;
    	q[quadlen].res = (char*)malloc(sizeof(char)*10);
    	strcpy(q[quadlen].op,"=");
    	strcpy(q[quadlen].arg1,st[top]);
    	strcpy(q[quadlen].res,$<s.str>1);
		top--;
    	quadlen++;
		}';' more_const_definition
		| error ';'
        ;

constant :
        T_INTVAL {push_value(yylval.s.type);}
        | T_BOOLVAL {push_value(yylval.s.type);}
        | T_FLOATVAL {push_value(yylval.s.type);}
        | T_STRINGVAL {push_value(yylval.s.type);}
        ;

more_const_definition :
        T_IDENTIFIER T_SINGLEEQ constant{
			struct symbol_table *s = NULL;
		HASH_FIND_STR(SYMBOL_TABLE,$<s.str>1, s);
		if(!s){
			s = malloc(sizeof(struct symbol_table));
			strcpy(s->type,yylval.s.type);
			s->scope_level = strdup("const");
			char var_mang_name[31];
			strcpy(var_mang_name, yylval.s.str);
			strcat(var_mang_name, "$");
			strcat(var_mang_name, s->scope_level);
			strcpy(s->var_name,var_mang_name);
			// printf("\nAlert : Inserting Variable '%s' in to the Symbol Table.\n", var_mang_name);
			s->line_no = yylloc.first_line;
			s->col_no = yylloc.first_column;
			if(yylval.s.intval!=0){
				s->var_value.int_value = $<s.intval>3;
				fprintf(fptr,"%s %d\n",yylval.s.str,yylval.s.intval);
				// printf("%s = %d",s->var_name,s->var_value.intval);
			}
			if(yylval.s.floatval!=0){				
				s->var_value.float_value = $<s.floatval>3;
				fprintf(fptr,"%s %f\n",yylval.s.str,yylval.s.floatval);
				// printf("%s = %f",s->var_name,s->var_value.intval);
			}
			if (yylval.s.stringval != NULL){
				fprintf(fptr,"%s %s\n",yylval.s.str,yylval.s.stringval);
			}

			HASH_ADD_STR(SYMBOL_TABLE, var_name, s);
			//printf("yayy\n");
		}
		// printf("%s %s %d %f \n",$<s.str>2,$<s.type>2,$<s.intval>2,$<s.floatval>2); 
		// printf("%s %s %d %f \n",$<s.str>3,$<s.type>3,$<s.intval>3,$<s.floatval>3); 
		$<s.type>1 = $<s.type>3;
		printf("%s = %s\n",$<s.str>1,st[top]);
		q[quadlen].op = (char*)malloc(sizeof(char));
    	q[quadlen].arg1 = (char*)malloc(sizeof(char)*10);
    	q[quadlen].arg2 = NULL;
    	q[quadlen].res = (char*)malloc(sizeof(char)*10);
    	strcpy(q[quadlen].op,"=");
    	strcpy(q[quadlen].arg1,st[top]);
    	strcpy(q[quadlen].res,$<s.str>1);
		top--;
    	quadlen++;
			
		}
		';' more_const_definition
        | error ';'
		|
        ;

type_block :
        T_TYPE type_definition
		{
			//printf("Type block\n");
			
		}
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
			char** new_types = (char**)malloc(sizeof(char*)* 10); // where is it used?
			for(int i = 0; i <= type_identifier_top; i++)
			{	
				new_types[i] = strdup(type_identifier_stack[i]);
			}
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
			struct type_node_info new_type_entry;
			new_type_entry.new_types = new_types;
			new_type_entry.type_identifier_top = type_identifier_top;
			new_type_entry.actual_type = yylval.s.type;
			type_node_stack[++type_node_stack_top] = new_type_entry;
			type_identifier_top = -1;
        }
        ';' type_definition
        | error ';'
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
		{
		}
        ;

variable_block :
        T_VAR decl_stmts
		{
			//printf("Variable block\n");
			//$$ = $2;
		}
        |
        ;

// here symbol table stuff will come ?
decl_stmts :
        T_IDENTIFIER
        {
            var_name_stack_top++;
			var_name_stack[var_name_stack_top] = strdup(yylval.s.str);
        }
        more_decl_stmt ':' data_type  ';' decl_stmts 
        | error ';'
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
					yyerror("Variable already declared.");
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
					yyerror("Variable already declared.");
				}
			}
			else
			{
				char error[1000];
				sprintf(error, "Data type %s is not defined.", yylval.s.str);
				yyerror(error);
			}
        }
        | T_ARRAY '[' T_INDEXTYPE ']' T_OF T_DATATYPE
		{
			//printf("Hit the type part of line %s\n", yylval.type);
			int result = dump_stack_in_symbol_table("array", yylloc.first_line, yylloc.first_column);
			if(!result){
				yyerror("Variable already declared.");
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
			//printf("Entering the Procedure %s\n", curr_scope_level);
		}
		';'  block ';'
		| T_PROCEDURE T_IDENTIFIER 
		{
			curr_scope_level = strdup(yylval.s.str);
			//printf("Entering the Procedure %s\n", curr_scope_level);
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
			//printf("Entering the Function %s\n", curr_scope_level);
			struct symbol_table *s = NULL;
			HASH_FIND_STR(SYMBOL_TABLE,$<s.str>2, s);
			if(!s){
				s = malloc(sizeof(struct symbol_table));
				strcpy(s->type,"function");
				s->scope_level = strdup(curr_scope_level);
				char var_mang_name[31];
				strcpy(var_mang_name, yylval.s.str);
				strcat(var_mang_name, "$");
				strcat(var_mang_name, s->scope_level);
				strcpy(s->var_name,var_mang_name);
				// printf("\nAlert : Inserting Variable '%s' in to the Symbol Table.\n", var_mang_name);
				s->line_no = yylloc.first_line;
				s->col_no = yylloc.first_column;
				HASH_ADD_STR(SYMBOL_TABLE, var_name, s);
				//printf("yayy\n");
			}
			else {
				printf("BLTHR\n");
			}
			
		}
		':' T_DATATYPE ';' block ';' 
		{
			strcpy(curr_scope_level,"global");
		}
		| T_FUNCTION T_IDENTIFIER 
		{
			curr_scope_level = strdup(yylval.s.str);

			struct symbol_table *s = NULL;
			HASH_FIND_STR(SYMBOL_TABLE,$<s.str>2, s);
			if(!s){
				s = malloc(sizeof(struct symbol_table));
				strcpy(s->type,"function");
				s->scope_level = strdup(curr_scope_level);
				char var_mang_name[31];
				strcpy(var_mang_name, yylval.s.str);
				strcat(var_mang_name, "$");
				strcat(var_mang_name, s->scope_level);
				strcpy(s->var_name,var_mang_name);
				// printf("\nAlert : Inserting Variable '%s' in to the Symbol Table.\n", var_mang_name);
				s->line_no = yylloc.first_line;
				s->col_no = yylloc.first_column;
				HASH_ADD_STR(SYMBOL_TABLE, var_name, s);
				//printf("yayy\n");
			}
			else {
				printf("BLTHR\n");
			}
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
				yyerror("Variable already declared.");
			}
		}
		function_param_continue
		;

function_param_continue :
		';' function_param_list
		| error ';'
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
		| error ';' 
		;

statements :
		execution_block
		| {l = 0;}assignment_statements	
        | if_statement 
        | fordo_statement
        | procedure_call_statements
        |
        ;
	
procedure_call_statements:
		T_PROCALL {push_symbol("print");} actuals {call_function();}
		;

actuals :
		'(' {in_write = 1;	write_param_count = 0;}
		expressionList
		{
			printf("Near write params\n");
			write_params(write_param_count);
			in_write = 0;
		} ')' 
		|
		;

expressionList :
		expression
		| expression ',' expressionList
		;

assignment_statements :
        T_IDENTIFIER 
        {
			push();
			if(!check_valid_identifier(yylval.s.str)){
				char error[1000];
				//printf("Scope Level : %s ",curr_scope_level);
				sprintf(error,"Variable %s is not declared.",yylval.s.str);
				yyerror(error);
			}
			else
			{
				assignment_name_stack_top++;
				assignment_name_stack[assignment_name_stack_top] = strdup(yylval.s.str);
			}
        }
        assignment_operators expression{
			codegen_assign();
			//printf("l%d\n",l);
			//printf("A%s\n", $<s.str>1);
			if(l == 1){
				// printf("A%s\n", $<s.type>4);
				type_stack_top++;
				type_stack[type_stack_top] = strdup($<s.type>4);
			}
			//printf("Assign%s",type_stack[type_stack_top]);
			check_assign(get_type($<s.str>1), type_stack[type_stack_top--]);			
			l=0;
		}
        ;

expression :
        simpleExpression
		{
			//printf("Assignment operation %s = %s\n",$<s.str>1,$<s.str>$);
			// $<s.type>$ = $<s.type>1;
			// $<s.intval>$ = $<s.intval>1;	
			// $<s.floatval>$ = $<s.floatval>1;	
			// $<s.str>$ = $<s.str>1;	
			//printf("Assignment operation %s = %s\n",$<s.str>$,$<s.str>0);
		}
        | simpleExpression T_SINGLEEQ{	push_symbol("==");} simpleExpression{codegen_assigna();}
		| simpleExpression T_NE  {	push_symbol("<>");}simpleExpression {codegen_assigna();}
		| simpleExpression '<' {push_symbol("<");} simpleExpression {codegen_assigna();}
		| simpleExpression T_LE {push_symbol("<=");} simpleExpression {codegen_assigna();}
		| simpleExpression '>' {push_symbol(">");} simpleExpression {codegen_assigna();}
		| simpleExpression T_GE {	push_symbol(">=");} simpleExpression {codegen_assigna();}
        {
                // not sure what this is so I left      
            //printf("%d and %d and %s\n",$<s.intval>1,$<s.intval>3,$<s.str>2);
			$<s.intval>$ = solution($<s.intval>1,$<s.intval>3,$<s.str>2);
			
			if(assignment_name_stack_top == -1) {
				break;
			}
			else 
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
					temp->var_value.int_value = $<s.intval>$;
					HASH_REPLACE_STR(SYMBOL_TABLE, var_name, temp, r);  /* var_name: name of key field */
					assignment_name_stack[assignment_name_stack_top--] = NULL;
				}
				else {
					char error[1000];
					//printf("Scope Level : %s ",curr_scope_level);
					sprintf(error,"Variable %s is not declared.",yylval.s.str);
					yyerror(error);
					// printf("---------------\n");
				}
			}
        }
        ;

simpleExpression :
		term 
		| simpleExpression '+'{push_symbol("+");} term 
		{
			codegen();	
			//printf("SE%s %s\n",$<s.str>1, $<s.str>4);
			//check_types($<s.str>1, $<s.str>4);
			if(strcmp("const",$<s.stringval>1)==0){
				if (strcmp("const",$<s.stringval>4)==0){
					check_t($<s.type>1, $<s.type>4);
				}
				else{
					char* t2 = get_type($<s.str>3);
					check_t($<s.type>1, t2);
				}
			}
			else if(strcmp("const",$<s.stringval>4)==0){
				char* t1 = get_type($<s.str>1);
				check_t( t1,  $<s.type>4);
			} 
			else{
				char* t1 = get_type($<s.str>1);
				char* t2 = get_type($<s.str>3);
				check_t(t1, t2);
			}
			
		}
		| simpleExpression '-'{push_symbol("-");} term {
			codegen();	
			if(strcmp("const",$<s.stringval>1)==0){
				if (strcmp("const",$<s.stringval>4)==0){
					check_t($<s.type>1, $<s.type>4);
				}
				else{
					char* t2 = get_type($<s.str>3);
					check_t($<s.type>1, t2);
				}
			}
			else if(strcmp("const",$<s.stringval>4)==0){
				char* t1 = get_type($<s.str>1);
				//printf("%s %s\n", t1,$<s.type>4);
				check_t( t1,  $<s.type>4);
			} 
			else{
				char* t1 = get_type($<s.str>1);
				char* t2 = get_type($<s.str>3);
				check_t(t1, t2);
			}
			}
		| simpleExpression T_BOOL_OR {push_symbol("||");} term {
			codegen();
			if(strcmp("const",$<s.stringval>1)==0){
				if (strcmp("const",$<s.stringval>4)==0){
					check_t($<s.type>1, $<s.type>4);
				}
				else{
					char* t2 = get_type($<s.str>3);
					check_t($<s.type>1, t2);
				}
			}
			else if(strcmp("const",$<s.stringval>4)==0){
				char* t1 = get_type($<s.str>1);
				printf("%s %s\n", t1,$<s.type>4);
				check_t( t1,  $<s.type>4);
			} 
			else{
				char* t1 = get_type($<s.str>1);
				char* t2 = get_type($<s.str>3);
				check_t(t1, t2);
			}
		
		}
		| simpleExpression '|' {push_symbol("|");}term {
			codegen();
			if(strcmp("const",$<s.stringval>1)==0){
				if (strcmp("const",$<s.stringval>4)==0){
					check_t($<s.type>1, $<s.type>4);
				}
				else{
					char* t2 = get_type($<s.str>3);
					check_t($<s.type>1, t2);
				}
			}
			else if(strcmp("const",$<s.stringval>4)==0){
				char* t1 = get_type($<s.str>1);
				//printf("%s %s\n", t1,$<s.type>4);
				check_t( t1,  $<s.type>4);
			} 
			else{
				char* t1 = get_type($<s.str>1);
				char* t2 = get_type($<s.str>3);
				check_t(t1, t2);
			}
		}
		| simpleExpression '!' {push_symbol("!");}term {
			codegen();	
			if(strcmp("const",$<s.stringval>1)==0){
				if (strcmp("const",$<s.stringval>4)==0){
					check_t($<s.type>1, $<s.type>4);
				}
				else{
					char* t2 = get_type($<s.str>3);
					check_t($<s.type>1, t2);
				}
			}
			else if(strcmp("const",$<s.stringval>4)==0){
				char* t1 = get_type($<s.str>1);
				//printf("%s %s\n", t1,$<s.type>4);
				check_t( t1,  $<s.type>4);
			} 
			else{
				char* t1 = get_type($<s.str>1);
				char* t2 = get_type($<s.str>3);
				check_t(t1, t2);
			}
		}
		{
			if(assignment_name_stack_top == -1) {
				break;
			}
			else 
            {    
				// not sure what this is so I left      
				//printf("%d and %d and %s\n",$<s.intval>1,$<s.intval>3,$<s.str>2);
				//$<s.intval>$ = solution($<s.intval>1,$<s.intval>3,$<s.str>2);
				
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
					assignment_name_stack[assignment_name_stack_top--] = NULL;
				}
				else {
					char error[1000];
					//printf("Scope Level : %s ",curr_scope_level);
					sprintf(error,"Variable %s is not declared.",yylval.s.str);
					yyerror(error);
					// printf("---------------\n");
				}
			}
        }
		;

term :
		factor	{$<s.stringval>$ = $<s.stringval>1;}
		| term '*' {push_symbol("*");}factor {
			codegen();	
			if(strcmp("const",$<s.stringval>1)==0){
				if (strcmp("const",$<s.stringval>4)==0){
					check_t($<s.type>1, $<s.type>4);
				}
				else{
					char* t2 = get_type($<s.str>3);
					check_t($<s.type>1, t2);
				}
			}
			else if(strcmp("const",$<s.stringval>4)==0){
				char* t1 = get_type($<s.str>1);
				//printf("%s %s\n", t1,$<s.type>4);
				check_t( t1,  $<s.type>4);
			} 
			else{
				char* t1 = get_type($<s.str>1);
				char* t2 = get_type($<s.str>3);
				check_t(t1, t2);
			}
		}			
		| term '/' {push_symbol("/");}factor {
			codegen();	
			if(strcmp("const",$<s.stringval>1)==0){
				if (strcmp("const",$<s.stringval>4)==0){
					check_t($<s.type>1, $<s.type>4);
				}
				else{
					char* t2 = get_type($<s.str>3);
					check_t($<s.type>1, t2);
				}
			}
			else if(strcmp("const",$<s.stringval>4)==0){
				char* t1 = get_type($<s.str>1);
				printf("%s %s\n", t1,$<s.type>4);
				check_t( t1,  $<s.type>4);
			} 
			else{
				char* t1 = get_type($<s.str>1);
				char* t2 = get_type($<s.str>3);
				check_t(t1, t2);
			}
		}
		| term '%' {push_symbol("%");}factor {
			codegen();	
			if(strcmp("const",$<s.stringval>1)==0){
				if (strcmp("const",$<s.stringval>4)==0){
					check_t($<s.type>1, $<s.type>4);
				}
				else{
					char* t2 = get_type($<s.str>3);
					check_t($<s.type>1, t2);
				}
			}
			else if(strcmp("const",$<s.stringval>4)==0){
				char* t1 = get_type($<s.str>1);
				//printf("%s %s\n", t1,$<s.type>4);
				check_t( t1,  $<s.type>4);
			} 
			else{
				char* t1 = get_type($<s.str>1);
				char* t2 = get_type($<s.str>3);
				check_t(t1, t2);
			}
		}
		| term T_BOOL_AND{push_symbol("&&");} factor {
			codegen();	
			if(strcmp("const",$<s.stringval>1)==0){
				if (strcmp("const",$<s.stringval>4)==0){
					check_t($<s.type>1, $<s.type>4);
				}
				else{
					char* t2 = get_type($<s.str>3);
					check_t($<s.type>1, t2);
				}
			}
			else if(strcmp("const",$<s.stringval>4)==0){
				char* t1 = get_type($<s.str>1);
				//printf("%s %s\n", t1,$<s.type>4);
				check_t( t1,  $<s.type>4);
			} 
			else{
				char* t1 = get_type($<s.str>1);
				char* t2 = get_type($<s.str>3);
				check_t(t1, t2);
			}
		}
		| term '&' {push_symbol("&");}factor {
			codegen();	
			if(strcmp("const",$<s.stringval>1)==0){
				if (strcmp("const",$<s.stringval>4)==0){
					check_t($<s.type>1, $<s.type>4);
				}
				else{
					char* t2 = get_type($<s.str>3);
					check_t($<s.type>1, t2);
				}
			}
			else if(strcmp("const",$<s.stringval>4)==0){
				char* t1 = get_type($<s.str>1);
				//printf("%s %s\n", t1,$<s.type>4);
				check_t( t1,  $<s.type>4);
			} 
			else{
				char* t1 = get_type($<s.str>1);
				char* t2 = get_type($<s.str>3);
				check_t(t1, t2);
			}
		}
		{
			if (assignment_name_stack_top == -1) {
				break;
			}
			else 
            {
				// not sure what this is so I left      
				//printf("%d and %d and %s\n",$<s.intval>1,$<s.intval>3,$<s.str>2);
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
					assignment_name_stack[assignment_name_stack_top--] = NULL;
				}
				else {
					char error[1000];
					//printf("Scope Level : %s ",curr_scope_level);
					sprintf(error,"Variable %s is not declared.",yylval.s.str);
					yyerror(error);
					// printf("---------------\n");
				}
			}
        }
		;

factor :
		'(' expression ')'	{$<s.intval>$ = $<s.intval>2;}
		| '+'{push_symbol("+");} factor {codegen();} 	
		| '-'{ push_symbol("-");} factor{codegen();}
		| T_BOOL_NOT{push_symbol("~");} factor{codegen();}
		| value  { l += 1; $<s.stringval>$ = $<s.stringval>1;}
		| T_IDENTIFIER 
		{
			l += 1;
			//printf("P%s %s\n", $<s.type>1, $<s.str>1);
			write_param_count++;
			push();
			if(!check_valid_identifier(yylval.s.str)){
				char error[1000];
				//printf("Scope Level : %s ",curr_scope_level);
				sprintf(error,"Variable %s is not declared.",yylval.s.str);
				yyerror(error);
			}
        }

value :
        T_INTVAL
        {
			char* check = $<s.stringval>-1;
			//printf("%s\n", check);
			char* check2 = $<s.stringval>1;
			write_param_count++;
			push_value(yylval.s.type);
			if(assignment_name_stack_top == -1) {
				break;
			}
			else if ((strcmp(check,"\0")==0 || strcmp(check,"const")==0)&& strcmp(check2,"const")==0 )
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
					if(strcmp(s->type, "real")==0){
						temp->var_value.float_value = yylval.s.intval;
					}
					else{
						temp->var_value.int_value = yylval.s.intval;
					}
					HASH_REPLACE_STR( SYMBOL_TABLE, var_name, temp,r );  /* var_name: name of key field */
					assignment_name_stack[assignment_name_stack_top--] = NULL;
				}
				else {
					char error[1000];
					//printf("Scope Level : %s ",curr_scope_level);
					sprintf(error,"Variable %s is not declared.",yylval.s.str);
					yyerror(error);
			
				}
			}
        }
        | T_FLOATVAL
        {
			char* check = $<s.stringval>-1;
			char* check2 = $<s.stringval>1;
			write_param_count++;
			push_value(yylval.s.type);
			if(assignment_name_stack_top == -1) {
				break;
			}
			else if ((strcmp(check,"\0")==0 || strcmp(check,"const")==0)&& strcmp(check2,"const")==0 )
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
					assignment_name_stack[assignment_name_stack_top--] = NULL;
				}
				else {
					char error[1000];
					//printf("Scope Level : %s ",curr_scope_level);
					sprintf(error,"Variable %s is not declared.",yylval.s.str);
					yyerror(error);
				}
			}

        }
        | T_BOOLVAL
        {
			char* check = $<s.stringval>-1;
			char* check2 = $<s.stringval>1;
			write_param_count++;
			push_value(yylval.s.type);
			if(assignment_name_stack_top == -1) {
				break;
			}
			else if ((strcmp(check,"\0")==0 || strcmp(check,"const")==0)&& strcmp(check2,"const")==0 )
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
					assignment_name_stack[assignment_name_stack_top--] = NULL;
				}
				else {
					char error[1000];
					sprintf(error,"Variable %s is not declared.",yylval.s.str);
					yyerror(error);
					
				}
			}
        }
        | T_STRINGVAL
        {
			char* check = $<s.stringval>-1;
			char* check2 = $<s.stringval>1;
			write_param_count++;
			push_value(yylval.s.type);
			if(assignment_name_stack_top == -1) {
				break;
			}
			else if (!in_write && (strcmp(check,"\0")==0 || strcmp(check,"const")==0)&& strcmp(check2,"const")==0 )
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
					//printf("A%s\n", yylval.s.str);
					temp = malloc(sizeof(struct symbol_table));
					strcat(temp->var_name, var_mang_name);
					strcpy(temp->type, s->type);
					temp->scope_level = s->scope_level;
					temp->line_no = s->line_no;
					temp->col_no = s->col_no;
					strcpy(temp->var_value.string_value,yylval.s.str);
					HASH_REPLACE_STR( SYMBOL_TABLE, var_name, temp,r );  /* var_name: name of key field */
					assignment_name_stack[assignment_name_stack_top--] = NULL;
				}
				else {
					char error[1000];
					sprintf(error,"Variable %s is not declared.",yylval.s.str);
					yyerror(error);
				}
			}
        }
        ;

assignment_operators :
        T_ASOP{
			push_symbol("=");
		}
        | T_AS_PE {push_symbol("+=");}
        | T_AS_SE {push_symbol("-=");}
        | T_AS_MULE{push_symbol("*=");}
        | T_AS_DIVE{push_symbol("/=");}
        ;
if_statement :
		T_IF expression {if1();} T_THEN statements {if2();}else_
		;
else_:
		T_ELSE statements {if3();}
		|
		;

fordo_statement :
        T_FOR T_IDENTIFIER {push();} T_ASOP {push_symbol(":=");} expression { codegen_assign();} to_or_downto expression
		{
			for1();
			for2(); 
			for3($<s.str>2);
		} T_DO statements
		{
			for4();
		}
to_or_downto :
        T_TO {push_symbol("<");}
        | T_DOWNTO {push_symbol(">");}
        ;

%%
int yyerror(const char *message) {
	printf("\033[0;31m");
	printf("\n\nInvalid Syntax:%d:%d Reason being %s\n",yylloc.first_line,yylloc.first_column,message);
	printf("\033[0;37m");
	successful=0;
	return 0;
}

// char st[100][100];

char i_[2]="0";
int temp_i=0;
char tmp_i[3];
char temp[4]="t";
int label[20];
int lnum=0;
int ltop=0;
int abcd=0;
int l_while=0;
int l_for=0;
int flag_set = 1;
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
		printf("\033[0;32m");
		printf("\n\nCompiled Successfully\n");
		printf("Took : %lf seconds\n", time_elapsed(&start, &end));

		printf("\033[0;37m");
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
		FILE *tacfptr;
		char* tac_file_name = (char*)malloc(sizeof(char)*20);
		strcpy(tac_file_name,"generated_tac.tac");
		tacfptr = fopen(tac_file_name,"w");
		for(i=0;i<quadlen;i++)
		{
        fprintf(tacfptr,"%-8s \t %-8s \t %-8s \t %-6s \n",q[i].op,q[i].arg1,q[i].arg2,q[i].res);
		}
		fclose(fptr);
		fclose(tacfptr);
	}
	else {
		printf("\033[0;37m");
		printf("\nOH NO! Compilation Failed! :( \n");
	}

}

double time_elapsed(struct timespec *start, struct timespec *end) {
	double t;
	t = (end->tv_sec - start->tv_sec); // diff in seconds
	t += (end->tv_nsec - start->tv_nsec) * 0.000000001; //diff in nanoseconds
	return t;
}

char* get_type(char* op1){
	struct symbol_table *s1 = NULL;
	char var_mang_name1[31];
	strcpy(var_mang_name1, op1);
	strcat(var_mang_name1, "$");
	strcat(var_mang_name1, curr_scope_level);
	HASH_FIND_STR(SYMBOL_TABLE, var_mang_name1, s1);
	if(s1){
		return s1->type;
	}
	else{
		strcpy(var_mang_name1, op1);
		strcat(var_mang_name1, "$");
		strcat(var_mang_name1, "const");
		HASH_FIND_STR(SYMBOL_TABLE, var_mang_name1, s1);
		if(s1){
			return s1->type;
		}
	}
}
/*void check_types(char* op1, char* op2)
{
	printf("Types%s %s\n", op1, op2);
	struct symbol_table *s1 = NULL;	
	struct symbol_table *s2 = NULL;	
	char var_mang_name1[31];
	char var_mang_name2[31];
	strcpy(var_mang_name1, op1);
	strcat(var_mang_name1, "$");
	strcat(var_mang_name1, curr_scope_level);
	strcpy(var_mang_name2, op2);
	strcat(var_mang_name2, "$");
	strcat(var_mang_name2, curr_scope_level);
	HASH_FIND_STR(SYMBOL_TABLE, var_mang_name1, s1);
	HASH_FIND_STR(SYMBOL_TABLE, var_mang_name2, s2);
	// check if both symbols are present in the symbol table
	//printf("%s %s",s1->type,s2->type);
	if (s1 && s2){
		printf("A%s %s",s1->type,s2->type);
		if (strcmp(s1->type, s2->type) !=0)
		{
		

			if (strcmp(s1->type,"integer")==0 && strcmp(s2->type,"integer")==0){
				//printf("Adding 2 integers\n");
				
			}

			if (strcmp(s1->type,"integer")==0 && strcmp(s2->type,"real")==0){
				printf("Warning: Integer type and Real type, typecasting 1st number\n");
				
			}

			if (strcmp(s1->type,"real")==0 && strcmp(s2->type,"integer")==0){
				printf("Warning: Real type and Integer type, typecasting 2st number\n");
				
			}
			if ((strcmp(s1->type,"integer")==0 && strcmp(s2->type,"boolean")==0) || (strcmp(s1->type,"boolean")==0 && strcmp(s2->type,"integer")==0) ){
				// printf("Adding 2 integers, typecasting 2st number\n");
				// printf("Cannot add int and boolean, aborting\n");	
				yyerror("Warning: Cannot operate int and boolean, aborting\n\n");	
			}

			if ((strcmp(s1->type,"integer")==0 && strcmp(s2->type,"string")==0) || (strcmp(s1->type,"string")==0 && strcmp(s2->type,"integer")==0) ){
				// printf("Adding 2 integers, typecasting 2st number\n");
				// printf("Cannot add int and boolean, aborting\n");	
				yyerror("Warning: Cannot operate int and string, aborting\n\n");	
			}
			}
			else{
			yyerror("Abort: Incompatible Datatypes.");
		}
	}
	//else if(strcmp("const",$<s.stringval>1)==0){

	//}
}*/

void check_t(char* t1, char* t2){
	//printf("IN CHECK_T%s %s\n", t1,t2);
	if (strcmp(t1,"integer")==0 && strcmp(t2,"integer")==0){
		
		type_stack_top++;
		type_stack[type_stack_top] = strdup(t1);
		//printf("Adding 2 integers\n");
	}
	else if (strcmp(t1,"real")==0 && strcmp(t2,"real")==0){
		
		type_stack_top++;
		type_stack[type_stack_top] = strdup(t1);
		//printf("Adding 2 integers\n");
	}
	else if (strcmp(t1,"string")==0 && strcmp(t2,"string")==0){
		type_stack_top++;
		type_stack[type_stack_top] = strdup(t1);
		//printf("Adding 2 integers\n");
	}
	else if (strcmp(t1,"integer")==0 && strcmp(t2,"real")==0){
		
		type_stack_top++;
		type_stack[type_stack_top] = strdup(t2);
		printf("Warning: Integer type and Real type, typecasting 1st number\n");
		
	}

	else if (strcmp(t1,"real")==0 && strcmp(t2,"integer")==0){
		
		type_stack_top++;
		type_stack[type_stack_top] = strdup(t1);
		printf("Warning: Real type and Integer type, typecasting 2st number\n");
		
	}
	else if ((strcmp(t1,"integer")==0 && strcmp(t2,"boolean")==0) || (strcmp(t1,"boolean")==0 && strcmp(t2,"integer")==0) ){
		// printf("Adding 2 integers, typecasting 2st number\n");
		// printf("Cannot add int and boolean, aborting\n");	
		yyerror("Warning: Cannot operate int and boolean, aborting\n\n");	
	}

	else if ((strcmp(t1,"integer")==0 && strcmp(t2,"string")==0) || (strcmp(t1,"string")==0 && strcmp(t2,"int")==0) ){
		// printf("Adding 2 integers, typecasting 2st number\n");
		// printf("Cannot add int and boolean, aborting\n");	
		yyerror("Warning: Cannot operate int and string, aborting\n\n");	
	}
	
	else{
		yyerror("Abort: Incompatible Datatypes.");
	}
}

void check_assign(char* t1, char* t2)
{
	if (strcmp(t1,"integer")==0 && strcmp(t2,"integer")==0){
		
	}
	else if (strcmp(t1,"real")==0 && strcmp(t2,"real")==0){
		
	}
	else if (strcmp(t1,"string")==0 && strcmp(t2,"string")==0){
		
	}
	else if (strcmp(t1,"boolean")==0 && strcmp(t2,"boolean")==0){
		
	}
	else if (strcmp(t1,"integer")==0 && strcmp(t2,"real")==0){
		yyerror("Abort: Incompatible Datatypes. Expecting integer got real\n\n");
	}

	else if (strcmp(t1,"real")==0 && strcmp(t2,"integer")==0){
		printf("Warning: Real type and Integer type, typecasting 2st number\n\n");
		
	}
	else if (strcmp(t1,"integer")==0 && strcmp(t2,"boolean")==0){
		yyerror("Abort: Incompatible Datatypes.Expecting boolean got integer\n\n");	
	}
	else if (strcmp(t1,"boolean")==0 && strcmp(t2,"integer")==0){
		yyerror("Abort: Incompatible Datatypes.Expecting integer got boolean\n\n");	
	}
	else if ((strcmp(t1,"integer")==0 && strcmp(t2,"string")==0) || (strcmp(t1,"string")==0 && strcmp(t2,"int")==0) ){
		// printf("Adding 2 integers, typecasting 2st number\n");
		// printf("Cannot add int and boolean, aborting\n");	
		yyerror("Abort: Incompatible Datatypes. Cannot operate on integer and string\n\n");	
	}
	else{
		yyerror("Abort: Incompatible Datatypes.");
	}	
}

void push()
{
	strcpy(st[++top],yylval.s.str);
}

void push_symbol(char* symbol)
{
	strcpy(st[++top],symbol);
}

void push_value(char* type){
	if( strcmp(type, "integer")==0){
		char s[10];
		sprintf(s, "%d", yylval.s.intval);
		strcpy(st[++top],s);
	}
	if( strcmp(type, "real")==0){
		char s[10];
		sprintf(s, "%f", yylval.s.floatval);
		strcpy(st[++top],s);
	}
	if( strcmp(type, "boolean")==0){
		char s[10];
		sprintf(s, "%d", yylval.s.intval);
		strcpy(st[++top],s);
	}
	if(strcmp(type, "string")==0){
		strcpy(st[++top],yylval.s.str);
	}

}
void codegen()
{
    strcpy(temp,"T");
    sprintf(tmp_i, "%d", temp_i);
    strcat(temp,tmp_i);
    printf("%s = %s %s %s\n",temp,st[top-2],st[top-1],st[top]);
    q[quadlen].op = (char*)malloc(sizeof(char)*(strlen(st[top-1])+1));
    q[quadlen].arg1 = (char*)malloc(sizeof(char)*strlen(st[top-2]));
    q[quadlen].arg2 = (char*)malloc(sizeof(char)*strlen(st[top]));
    q[quadlen].res = (char*)malloc(sizeof(char)*strlen(temp));
    strcpy(q[quadlen].op,st[top-1]);
    strcpy(q[quadlen].arg1,st[top-2]);
    strcpy(q[quadlen].arg2,st[top]);
    strcpy(q[quadlen].res,temp);
    quadlen++;
    top-=2;
    strcpy(st[top],temp);	
temp_i++;
}
void for1()
{
    l_for = lnum;	
    printf("L%d: \n",lnum++);
    q[quadlen].op = (char*)malloc(sizeof(char)*6);
    q[quadlen].arg1 = NULL;
    q[quadlen].arg2 = NULL;
    q[quadlen].res = (char*)malloc(sizeof(char)*(lnum+2));
    strcpy(q[quadlen].op,"Label");
    char x[10];
    sprintf(x,"%d",lnum-1);
    char* l = (char*) malloc((strlen(x)+2)*sizeof(char));
	l[0] = 'L';
	l[1] = '\0';
    strcpy(q[quadlen].res,strcat(l,x));
    quadlen++;
	int i=0;
}

void for2()
{
	codegen();
	
    strcpy(temp,"T");
    sprintf(tmp_i, "%d", temp_i);
    strcat(temp,tmp_i);
    printf("%s = not %s\n",temp,st[top]);
    q[quadlen].op = (char*)malloc(sizeof(char)*4);
    q[quadlen].arg1 = (char*)malloc(sizeof(char)*strlen(st[top]));
    q[quadlen].arg2 = NULL;
    q[quadlen].res = (char*)malloc(sizeof(char)*strlen(temp));
    strcpy(q[quadlen].op,"not");
    strcpy(q[quadlen].arg1,st[top]);
    strcpy(q[quadlen].res,temp);
    quadlen++;
    printf("if %s goto L%d\n",temp,lnum);
    q[quadlen].op = (char*)malloc(sizeof(char)*3);
    q[quadlen].arg1 = (char*)malloc(sizeof(char)*strlen(temp));
    q[quadlen].arg2 = NULL;
    q[quadlen].res = (char*)malloc(sizeof(char)*(lnum+2));
    strcpy(q[quadlen].op,"if");
    strcpy(q[quadlen].arg1,temp);
    char x[10];
    sprintf(x,"%d",lnum);
    char* l = (char*) malloc((strlen(x)+2)*sizeof(char));
	l[0] = 'L';
	l[1] = '\0';
    strcpy(q[quadlen].res,strcat(l,x));
    quadlen++;

    temp_i++;
    label[++ltop]=lnum;
    lnum++;
    printf("goto L%d\n",lnum);
    q[quadlen].op = (char*)malloc(sizeof(char)*5);
    q[quadlen].arg1 = NULL;
    q[quadlen].arg2 = NULL;
    q[quadlen].res = (char*)malloc(sizeof(char)*(lnum+2));
    strcpy(q[quadlen].op,"goto");
    char x1[10];
    sprintf(x1,"%d",lnum);
    char* l1 = (char*) malloc((strlen(x1)+2)*sizeof(char));
	l1[0] = 'L';
	l1[1] = '\0';
    strcpy(q[quadlen].res,strcat(l1,x1));
    quadlen++;
    label[++ltop]=lnum;
    printf("L%d: \n",++lnum);

    q[quadlen].op = (char*)malloc(sizeof(char)*6);
    q[quadlen].arg1 = NULL;
    q[quadlen].arg2 = NULL;
    q[quadlen].res = (char*)malloc(sizeof(char)*(lnum+2));
    strcpy(q[quadlen].op,"Label");
    char x2[10];
    sprintf(x2,"%d",lnum);
    char* l2 = (char*) malloc((strlen(x2)+2)*sizeof(char));
	l2[0] = 'L';
	l2[1] = '\0';
    strcpy(q[quadlen].res,strcat(l2,x2));
    quadlen++;
 }
void for3(char* id)
{
	push_symbol(id);
	push_symbol("=");
	push_symbol(id);
	push_symbol("+");
	push_symbol("1");
	codegen();
	codegen_assign();
    int x;
    x=label[ltop--];
    printf("goto L%d \n",l_for);

    q[quadlen].op = (char*)malloc(sizeof(char)*5);
    q[quadlen].arg1 = NULL;
    q[quadlen].arg2 = NULL;
    q[quadlen].res = (char*)malloc(sizeof(char)*strlen(temp));
    strcpy(q[quadlen].op,"goto");
    char jug[10];
    sprintf(jug,"%d",l_for);
    char* l = (char*) malloc((strlen(jug)+2)*sizeof(char));
	l[0] = 'L';
	l[1] = '\0';
    strcpy(q[quadlen].res,strcat(l,jug));
    quadlen++;


    printf("L%d: \n",x);

    q[quadlen].op = (char*)malloc(sizeof(char)*6);
    q[quadlen].arg1 = NULL;
    q[quadlen].arg2 = NULL;
    q[quadlen].res = (char*)malloc(sizeof(char)*(x+2));
    strcpy(q[quadlen].op,"Label");
    char jug1[10];
    sprintf(jug1,"%d",x);
    char* l1 = (char*) malloc((strlen(jug1)+2)*sizeof(char));
	l1[0] = 'L';
	l1[1] = '\0';
    strcpy(q[quadlen].res,strcat(l1,jug1));
    quadlen++;

}

void for4()
{
    int x;
    x=label[ltop--];
    printf("goto L%d \n",lnum);

    q[quadlen].op = (char*)malloc(sizeof(char)*5);
    q[quadlen].arg1 = NULL;
    q[quadlen].arg2 = NULL;
    q[quadlen].res = (char*)malloc(sizeof(char)*strlen(temp));
    strcpy(q[quadlen].op,"goto");
    char jug[10];
    sprintf(jug,"%d",lnum);
    char* l = (char*) malloc((strlen(jug)+2)*sizeof(char));
	l[0] = 'L';
	l[1] = '\0';
    strcpy(q[quadlen].res,strcat(l,jug));
    quadlen++;

    printf("L%d: \n",x);

    q[quadlen].op = (char*)malloc(sizeof(char)*6);
    q[quadlen].arg1 = NULL;
    q[quadlen].arg2 = NULL;
    q[quadlen].res = (char*)malloc(sizeof(char)*(x+2));
    strcpy(q[quadlen].op,"Label");
    char jug1[10];
    sprintf(jug1,"%d",x);
    char* l1 = (char*) malloc((strlen(jug1)+2)*sizeof(char));
	l1[0] = 'L';
	l1[1] = '\0';
    strcpy(q[quadlen].res,strcat(l1,jug1));
    quadlen++;
}

void codegen_assign()
{
	printf("%s %s %s \n", st[top-2], st[top-1], st[top]);
    q[quadlen].op = (char*)malloc(sizeof(char));
    q[quadlen].arg1 = (char*)malloc(sizeof(char)*strlen(st[top]));
    q[quadlen].arg2 = NULL;
    q[quadlen].res = (char*)malloc(sizeof(char)*strlen(st[top-3]));
    strcpy(q[quadlen].op,"=");
    strcpy(q[quadlen].arg1,st[top]);
    strcpy(q[quadlen].res,st[top-2]);
    quadlen++;
    top-=2;
}

void codegen_assigna()
{
strcpy(temp,"T");
sprintf(tmp_i, "%d", temp_i);
strcat(temp,tmp_i);
printf("%s = %s %s %s\n",temp,st[top-2],st[top-1],st[top]);

	q[quadlen].op = (char*)malloc(sizeof(char)*strlen(st[top-1]));
    q[quadlen].arg1 = (char*)malloc(sizeof(char)*strlen(st[top-2]));
    q[quadlen].arg2 = (char*)malloc(sizeof(char)*strlen(st[top]));
    q[quadlen].res = (char*)malloc(sizeof(char)*strlen(temp));
    strcpy(q[quadlen].op,st[top-1]);
    strcpy(q[quadlen].arg1,st[top-2]);
    strcpy(q[quadlen].arg2,st[top]);
    strcpy(q[quadlen].res,temp);
    quadlen++;
	top-=2;
	temp_i++;
	strcpy(st[top],temp);

}

void if1()
{
 lnum++;
 strcpy(temp,"T");
 sprintf(tmp_i, "%d", temp_i);
 strcat(temp,tmp_i);
 printf("%s = not %s\n",temp,st[top]);
 q[quadlen].op = (char*)malloc(sizeof(char)*4);
 q[quadlen].arg1 = (char*)malloc(sizeof(char)*strlen(st[top]));
 q[quadlen].arg2 = NULL;
 q[quadlen].res = (char*)malloc(sizeof(char)*strlen(temp));
 strcpy(q[quadlen].op,"not");
 strcpy(q[quadlen].arg1,st[top]);
 strcpy(q[quadlen].res,temp);
 quadlen++;
 printf("if %s goto L%d\n",temp,lnum);
 q[quadlen].op = (char*)malloc(sizeof(char)*3);
 q[quadlen].arg1 = (char*)malloc(sizeof(char)*strlen(temp));
 q[quadlen].arg2 = NULL;
 q[quadlen].res = (char*)malloc(sizeof(char)*(lnum+2));
 strcpy(q[quadlen].op,"if");
 strcpy(q[quadlen].arg1,st[top-2]);
 char x[10];
 sprintf(x,"%d",lnum);
 char* l = (char*) malloc((strlen(x)+2)*sizeof(char));
	l[0] = 'L';
	l[1] = '\0';
 strcpy(q[quadlen].res,strcat(l,x));
 quadlen++;

 temp_i++;
 label[++ltop]=lnum;
}

void if2(){
	int x;
    lnum++;
    x=label[ltop--];
    printf("goto L%d\n",lnum);
    q[quadlen].op = (char*)malloc(sizeof(char)*5);
    q[quadlen].arg1 = NULL;
    q[quadlen].arg2 = NULL;
    q[quadlen].res = (char*)malloc(sizeof(char)*(lnum+2));
    strcpy(q[quadlen].op,"goto");
    char jug[10];
    sprintf(jug,"%d",lnum);
    
	char* l = (char*) malloc((strlen(jug)+2)*sizeof(char));
	l[0] = 'L';
	l[1] = '\0';
    strcpy(q[quadlen].res,strcat(l,jug));
    quadlen++;
	printf("L%d: \n",x);
    q[quadlen].op = (char*)malloc(sizeof(char)*6);
    q[quadlen].arg1 = NULL;
    q[quadlen].arg2 = NULL;
    q[quadlen].res = (char*)malloc(sizeof(char)*(x+2));
    strcpy(q[quadlen].op,"Label");

    char jug1[10];
    sprintf(jug1,"%d",x);
	char* l1 = (char*) malloc((strlen(jug1)+2)*sizeof(char));
	l1[0] = 'L';
	l1[1] = '\0';
    strcpy(q[quadlen].res,strcat(l1,jug1));
    quadlen++;
    label[++ltop]=lnum;
}

void if3()
{
    int y;
    y=label[ltop--];
    printf("L%d: \n",y);
    q[quadlen].op = (char*)malloc(sizeof(char)*6);
    q[quadlen].arg1 = NULL;
    q[quadlen].arg2 = NULL;
    q[quadlen].res = (char*)malloc(sizeof(char)*(y+2));
    strcpy(q[quadlen].op,"Label");
    char x[10];
    sprintf(x,"%d",y);
	char* l = (char*) malloc((strlen(x)+2)*sizeof(char));
	l[0] = 'L';
	l[1] = '\0';
    strcpy(q[quadlen].res,strcat(l,x));
    quadlen++;
	++lnum;
}

void write_params(int n){
	for(int i=0;i<n;i++){
		if (st[top][0] != '\''){
			printf("param %s\n",st[top]);
			q[quadlen].op = (char*)malloc(sizeof(char)*11);
			q[quadlen].arg1 = (char*)malloc(sizeof(char)*strlen(st[top]));
			q[quadlen].arg2 = NULL;
			q[quadlen].res = NULL;
			strcpy(q[quadlen].op,"param");
			strcpy(q[quadlen].arg1,st[top]);
			quadlen++;
			top--;
		}
		else{
			strcpy(temp,"T");
   			sprintf(tmp_i, "%d", temp_i);
    		strcat(temp,tmp_i);
			printf("%s = %s \n",temp,st[top]);
			q[quadlen].op = (char*)malloc(sizeof(char)*11);
			q[quadlen].arg1 = (char*)malloc(sizeof(char)*strlen(st[top]));
			q[quadlen].res = (char*)malloc(sizeof(char)*strlen(temp));
			strcpy(q[quadlen].op, "=");
			strcpy(q[quadlen].arg1,st[top]);
			strcpy(q[quadlen].res,temp);
			quadlen++;
			
			strcpy(st[top],temp);	
			temp_i++;
			printf("param %s\n",st[top]);
			q[quadlen].op = (char*)malloc(sizeof(char)*11);
			q[quadlen].arg1 = (char*)malloc(sizeof(char)*strlen(st[top]));
			q[quadlen].arg2 = NULL;
			q[quadlen].res = NULL;
			strcpy(q[quadlen].op,"param");
			strcpy(q[quadlen].arg1,st[top]);
			quadlen++;
			top-=1;
		}
	}

	
}

void call_function(){
	strcpy(temp,"T");
    sprintf(tmp_i, "%d", temp_i);
    strcat(temp,tmp_i);
    printf("%s = call(%s, %d)\n",temp,st[top],write_param_count);
    q[quadlen].op = (char*)malloc(sizeof(char)*11);
    q[quadlen].arg1 = (char*)malloc(sizeof(char)*strlen(st[top]));
    q[quadlen].arg2 = (char*)malloc(sizeof(char)*10);
    q[quadlen].res = (char*)malloc(sizeof(char)*strlen(temp));
	char s[10];
	sprintf(s,"%d",write_param_count);
    strcpy(q[quadlen].op,"call");
    strcpy(q[quadlen].arg1,st[top]);
    strcpy(q[quadlen].arg2,s);
    strcpy(q[quadlen].res,temp);
    quadlen++;
    top--;
    strcpy(st[top],temp);	
	temp_i++;
}