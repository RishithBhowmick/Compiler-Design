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
        ProgramHeading Block
        ;

ProgramHeading :
        T_PROGRAM T_IDENTIFIER ';' newLineMaybe
        ;

newLineMaybe   :
        '\n'
        |
        ;

// just | followed by ; is for lambda

Block   :
        uses_block const_block type_block variable_block main_program_block
        ;

uses_block :
        T_USES T_IDENTIFIER other_libs ';' newLineMaybe
        |
        ;

other_libs :
        ',' other_libs 
        |
        ;

const_block :
        T_CONST onlyNewLine const_definition
        |
        ;

const_definition :
        T_IDENTIFIER "=" constant
        ;
constant :
        "integer"
        | "boolean"
        | "string"
        ;

type_block :
        T_TYPE onlyNewLine type_definition
        |
        ;
type_definition :
        T_IDENTIFIER = type
        ;

type :
        "character"
        | "integer"
        | "real"
        | "boolean"
        | "string"
        ;

variable_block :
        T_VAR onlyNewLine decl_stmts
        |
        ;

decl_stmts :
        decl_stmt onlyNewLine decl_stmts
        | decl_stmt
        ;

decl_stmt :
        T_IDENTIFIER ":" T_TYPE ";"
        ;

main_program_block :
        T_BEGIN onlyNewLine main_body onlyNewLine T_END "."
        ;

main_body :
        assignment_statements onlyNewLine main_body
        | if_statement onlyNewLine main_body
        | fordo_statement onlyNewLine main_body
        | print_statements onlyNewLine main_body
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
        T_IF condition_statement T_THEN onlyNewLine main_body
        | T_IF condition_statement T_THEN onlyNewLine main_body T_ELSE onlyNewLine main_body
        ;

fordo_statement :
        T_FOR T_IDENTIFIER T_ASOP value to_or_downto value T_DO newLineMaybe for_body
        ;

to_or_downto :
        T_TO
        | T_DOWNTO
        ;

for_body :
        