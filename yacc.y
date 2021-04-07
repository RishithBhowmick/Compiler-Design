%{
    #include<stdio.h>
    #include<stdlib.h>
    #include<time.h>
	#include<string.h>
    #include "uthash/src/uthash.h"
    #include "sym_tab.h" 
	#include<ast.h>
    #define YYPARSE_PARAM scanner
    #define YYLEX_PARAM   scanner

	#define count 5
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

	//functions for AST Tree
	node* construct_AST(node* left, node* right, char* token){
		node* newnode = (node*)malloc(sizeof(node));
		char* newstr = (char*)malloc(strlen(token)+1);
		strcpy(newstr, token);
		newnode->left = left;
		newnode->right = right;
		newnode->token = token;
		return newnode; 
	}

	void disp(node* root, int space){
		if(root == NULL){
			return
		}
		space += count;
		disp(root-> right, space);
		cout << endl;
		for(int i = count; i < space; i++){
			cout << " ";
		}
		cout << root->token << endl;
		disp(root->left; space);
	}

	void DisplayTree(node* tree){
		disp(tree, 0);
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
		struct ast_node* ast;
	}s;
}

// %nonassoc T_IFX
// %nonassoc T_ELSE

%token T_PROGRAM;
%token <str> T_IDENTIFIER;
%token T_USES;
%token T_TYPE;
%token T_VAR;
%token T_BEGIN;
%token T_END;
%token T_FUNCTION;
%token T_PROCEDURE;
%token T_FORWARD;
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

%token T_INDEXTYPE;

%token T_SINGLEEQ;
%token T_DIV;
%token T_MOD;
%token T_DUBDOT;

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

Program :
        ProgramHeader Declarations CompoundStatement '.'	{DisplayTree($<s.ast>1)}
        ;

ProgramHeader :
        T_PROGRAM T_IDENTIFIER ';'
        | T_PROGRAM T_IDENTIFIER '(' IdList ')' ';'
        ;

Declarations :
        ConstantDefinitions TypeDefinitions VariableDeclarations ProcedureDeclarations
        ;

epsilon :
        ;

ConstantDefinitions :
        T_CONST ConstantDefinitionList
        | epsilon
        ;

ConstantDefinitionList :
        ConstantDef
        | ConstantDef ConstantDefinitionList
        ;

ConstantDef :
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
                }
                if(yylval.s.floatval!=0){
                    s->var_value.float_value = $<s.floatval>3;
                }

                HASH_ADD_STR(SYMBOL_TABLE, var_name, s);
                //printf("yayy\n");
            }
            // printf("%s %s %d %f \n",$<s.str>2,$<s.type>2,$<s.intval>2,$<s.floatval>2); 
            // printf("%s %s %d %f \n",$<s.str>3,$<s.type>3,$<s.intval>3,$<s.floatval>3); 
        } ';'
        ;

TypeDefinitions :
        T_TYPE TypeDefinitionList
        | epsilon
        ;

TypeDefinitionList :
        TypeDef
        | TypeDef TypeDefinitionList
        ;

TypeDef :
        T_IDENTIFIER 
        {
            type_identifier_top++;
			type_identifier_stack[type_identifier_top] = strdup(yylval.s.str);
        }
        T_SINGLEEQ TypeSpecifier ';'
        ;

VariableDeclarations :
        T_VAR VariableDeclarationList
        | epsilon
        ;

VariableDeclarationList :
        VariableDec
        | VariableDec VariableDeclarationList
        ;

VariableDec :
        IdList ':' TypeSpecifier ';'
        ;

ProcedureDeclarations :
        ProcedureDec ProcedureDeclarations
        | epsilon
        ;

ProcedureDec :
        ProcedureHeader T_FORWARD ';'
        | ProcedureHeader Declarations CompoundStatement ';'
        | FunctionHeader T_FORWARD ';'
        | FunctionHeader Declarations CompoundStatement ';'
        ;

ProcedureHeader :
        T_PROCEDURE T_IDENTIFIER Arguments ';'
        ;

FunctionHeader :
        T_FUNCTION T_IDENTIFIER Arguments ':' TypeSpecifier ';'
        ;

Arguments :
        '(' ArgumentList ')'
        | epsilon
        ;

ArgumentList :
        Arg
        | Arg ';' ArgumentList
        ;

Arg :
        IdList ':' TypeSpecifier
        | T_VAR IdList ':' TypeSpecifier
        ;

CompoundStatement :
        T_BEGIN StatementList T_END
        ;

StatementList :
        Statement
        | Statement ';' StatementList
        ;

Statement :
        CompoundStatement
        | AssignemntStatement
        | ProcedureCall
        | ForStatement
        | IfStatement
        | epsilon
        ;

AssignemntStatement :
        Variable T_ASOP Expression
        | Variable T_AS_SE Expression
        | Variable T_AS_PE Expression
        | Variable T_AS_MULE Expression
        | Variable T_AS_DIVE Expression
        ;

ProcedureCall :
        T_IDENTIFIER Actuals
        ;

ForStatement :
        T_FOR T_IDENTIFIER T_ASOP Expression T_TO Expression T_DO Statement
        | T_FOR T_IDENTIFIER T_ASOP Expression T_DOWNTO Expression T_DO Statement
        ;

IfStatement :
        T_IF Expression T_THEN Statement %prec T_IFX
		| T_IF Expression T_THEN Statement T_ELSE Statement
        ;

Expression :
        SimpleExpression
        | SimpleExpression T_SINGLEEQ SimpleExpression
        | SimpleExpression T_NE SimpleExpression
        | SimpleExpression '<' SimpleExpression
        | SimpleExpression T_LE SimpleExpression
        | SimpleExpression '>' SimpleExpression
        | SimpleExpression T_GE SimpleExpression
        ;

SimpleExpression :
        Term 
        | SimpleExpression '+' Term
        | SimpleExpression '-' Term
        | SimpleExpression T_BOOL_OR Term
        ;

Term :
        Factor
        | Term '*' Factor
        | Term '/' Factor
        | Term T_DIV Factor
        | Term T_MOD Factor
        | Term T_BOOL_AND Factor
        ;

Factor :
        '(' Expression ')'
        | '+' Factor
        | '-' Factor
        | T_BOOL_NOT Factor
        | constant
        | Variable
        ;

Actuals :
        '(' ExpressionList ')'
        ;

ExpressionList :
        Expression
        | Expression ',' ExpressionList
        ;

Variable :
        T_IDENTIFIER
        | Variable '.' T_IDENTIFIER
        | Variable '^'
        | Variable '[' ExpressionList ']'
        ;

TypeSpecifier :
        T_IDENTIFIER
        | '^' TypeSpecifier
        | '(' IdList ')'
        | constant T_DUBDOT constant
        | T_ARRAY '[' DimensionList ']'
        ;

DimensionList :
        Dimension
        | Dimension ',' DimensionList
        ;

Dimension :
        constant T_DUBDOT constant
        |  T_IDENTIFIER
        ;

IdList : 
        T_IDENTIFIER {
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
        | T_IDENTIFIER 
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
        } ',' IdList
        ;

constant :
        T_INTVAL {
            if(assignment_name_stack_top == -1) {
				break;
			}
			else {
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
					//printf("Scope Level : %s ",curr_scope_level);
					sprintf(error,"Variable %s is not declared.",yylval.s.str);
					yyerror(error);
					// printf("---------------\n");
				}
			}
        }
        | T_BOOLVAL {
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
					temp->var_value.int_value = yylval.s.intval;
					HASH_REPLACE_STR( SYMBOL_TABLE, var_name, temp,r );  /* var_name: name of key field */
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
        | T_FLOATVAL {
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
					temp->var_value.float_value = yylval.s.floatval;
					HASH_REPLACE_STR( SYMBOL_TABLE, var_name, temp,r );  /* var_name: name of key field */
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
        | T_STRINGVAL {
            if(assignment_name_stack_top == -1) {
				break;
			}
			else {
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