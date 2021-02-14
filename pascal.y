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
        T_IDENTIFIER T_S