%{
    #include<stdio.h>
    int yylex();
    void yyerror(char *s);

    union data {
		int int_value;
		float float_value;
		char string_value[256];
    };

	struct symbol_table {
		char var_name[31]; //Holds the Name of the Identifier
		char type[10]; //Holds the DataType of Identifier
		char *scope_level;
		//int current_size; //Size of the Symbol Table
		int line_no;
		int col_no;
		union data var_value;
    };
%}
%token T_PROGRAM;
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
