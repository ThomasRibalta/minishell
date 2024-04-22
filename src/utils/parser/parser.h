#ifndef PARSER
# define PARSER

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>

// lexer
typedef enum {
    TOKEN_PAREN,
    TOKEN_COMMAND,
    TOKEN_LOGICAL_AND,
    TOKEN_LOGICAL_OR,
    TOKEN_PIPE,
	TOKEN_HEREDOC,
    TOKEN_REDIRECTION_IN,
    TOKEN_REDIRECTION_OUT,
    TOKEN_REDIRECTION_APPEND,
} TokenType;

typedef struct Token {
    TokenType type;
    char* value;
    struct Token* next;
} Token;

// parser
//Token *lexer();

// Define node types
typedef enum {
    NODE_COMMAND,
	NODE_EMPTY_COMMAND,
	NODE_PARENTHESE,
    NODE_LOGICAL_AND,
    NODE_LOGICAL_OR,
    NODE_PIPE,
    NODE_LOGICAL_HOLDER, // Holder for logical structure
} NodeType;

typedef struct Redirection {
    char* filename;
    struct Redirection* next;
} Redirection;

typedef struct ASTNode {
    NodeType type;
    char* value;
    struct ASTNode* left;
    struct ASTNode* right;
	Redirection* here_doc;
    Redirection* inputs;
    Redirection* outputs;
    Redirection* appends;
	bool builtIn;
} ASTNode;

typedef struct LogicalNode {
    NodeType type; // NODE_LOGICAL_AND, NODE_LOGICAL_OR, or NODE_LOGICAL_HOLDER
    struct ASTNode *left; // Left subtree (commands and pipes)
    struct ASTNode *right; // Right subtree (commands and pipes)
} LogicalNode;

// Define StartNode structure
typedef struct StartNode {
    LogicalNode **children; // Array of pointers to child nodes (logical AND/OR nodes)
    int childCount; // Number of children
    bool hasLogical; // Boolean to indicate if there are logical nodes
} StartNode;

void	free_lexer(Token **lexer);
void printEntireAST(const StartNode* startNode);
void freeRedirectionList(Redirection** list);
void expenser(StartNode* startNode);

#endif
