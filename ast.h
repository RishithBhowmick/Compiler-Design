typedef struct ast_node{
	char* token;
	struct ast_node* left;
	struct ast_node* right;
}ast_node;

struct ast_root{
	int node_type;
	struct ast_node* program_node;
	struct ast_node* block_node;
};

struct ast_program{
	int node_type;
	char* program_node;
};

struct ast_block{
	struct ast_node* uses_node;
	struct ast_node* const_node;
	struct ast_node* type_node;
	struct ast_node* var_node;
	// struct ast_node* list_func_proc_node;   //need to decide whther together or seperate
	struct ast_node* execution_node; 
};

struct ast_uses{
	int node_type;
	char** library_names;
	int no_libraries;
};

//different constants possible


struct ast_type{
	int node_type;
	struct ast_node* next_typedef;
};

struct ast_typedef{
	int node_type;
	char** new_types;
	char* type;
	struct ast_node* next_typedef;
};

struct ast_var{
	int node_type;
	struct ast_node* next_var_list;
};

struct ast_var_list{
	int node_type;
	char** new_vars;
	char* data_type;
	struct ast_node* next_var_list;
};

struct ast_execution_body{
	int node_type;
	struct ast_node* exec_body;
	struct ast_node* statements;
};

struct ast_assignment{
	int node_type;
	//
};

struct ast_if{
	int node_type;
	struct ast_node* if_body;
	struct ast_node* else_body;
	struct ast_node* condition;
};

struct ast_for{
	//figure out number of children
};

// void disp(node* ,int);