typedef struct ast_node{
	char* token;
	struct ast_node* left;
	struct ast_node* right;
} node;

node* construct_AST(node*, node*, char*);

void DisplayTree(node* );

void disp(node* ,int);