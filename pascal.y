%{
    #include<stdio.h>
    int yylex();
    void yyerror(char *s);

    union data {    // since value is not of a single type. Union is the best structure.
		int int_value;
		float float_value;
		char string_value[256];
    };

	struct symbol_table {
		char var_name[31];  // Holds the Name of the Identifier
		char type[10];    // Holds the DataType of Identifier
		char *scope_level;
		int line_no;
		int col_no;
		union data var_value;
    };
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
        T_USES T_IDENTIFIER other_libs ';'
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
        T_IDENTIFIER more_type_identifiers T_SINGLEEQ T_DATATYPE ';' type_definition
        |
        ;

more_type_identifiers :
        ',' T_IDENTIFIER more_type_identifiers
        |
        ;

variable_block :
        T_VAR decl_stmts
        |
        ;

// here symbol table stuff will come
decl_stmts :
        decl_stmt decl_stmts
        | decl_stmt
        ;

decl_stmt :
        T_IDENTIFIER ":" T_TYPE ";"
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
        T_IDENTIFIER assignment_operators expression
        ;

expression :
        T_IDENTIFIER
        | value
        | "(" expression ")"
        | expression operator expression
        ;

value :
        T_INTVAL
        | T_FLOATVAL
        | T_BOOLVAL
        | T_STRINGVAL
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
        T_IF condition_statement T_THEN execution_body
        | T_IF condition_statement T_THEN execution_body T_ELSE execution_body
        ;

fordo_statement :
        T_FOR T_IDENTIFIER T_ASOP expression to_or_downto expression T_DO execution_body
        ;

to_or_downto :
        T_TO
        | T_DOWNTO
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