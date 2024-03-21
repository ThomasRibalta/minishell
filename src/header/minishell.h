#ifndef MINISHELL_H
# define MINISHELL_H

# include "../utils/libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <signal.h>
# include <sys/wait.h>
# include <unistd.h>
# include <stdbool.h>
# define VIOLET_FONCE "\e[35;2m"
# define VIOLET "\e[35m"
# define VERT "\e[32m"
# define BLEU "\e[34m"
# define RESET "\e[0m"

typedef enum {
    TOKEN_PAREN, // (
    TOKEN_COMMAND, // Command name
    TOKEN_LOGICAL_AND, // &&
    TOKEN_LOGICAL_OR, // ||
    TOKEN_PIPE, // |
    TOKEN_REDIRECTION_IN, // <
    TOKEN_REDIRECTION_OUT, // >
    TOKEN_REDIRECTION_APPEND, // >>
} TokenType;

typedef struct Token {
    TokenType type;
    char* value;
    struct Token* next;
} Token;

// Define node types
typedef enum {
    NODE_COMMAND,
	NODE_PARENTHESE,
    NODE_LOGICAL_AND,
    NODE_LOGICAL_OR,
    NODE_PIPE,
    NODE_LOGICAL_HOLDER, // Holder for logical structure
} NodeType;

// Define ASTNode structure
typedef struct ASTNode {
    NodeType type;
    char *value; // Stores the command or argument
    struct ASTNode *left; // Used for binary trees (commands and pipes)
    struct ASTNode *right; // Chaining arguments or operations
} ASTNode;

typedef struct LogicalNode {
    NodeType type; // NODE_LOGICAL_AND, NODE_LOGICAL_OR, or NODE_LOGICAL_HOLDER
    char *leftInput;
	char *leftOutput;
	char *leftAppend;
    char *rightInput;
	char *rightOutput;
	char *rightAppend;
    struct ASTNode *left; // Left subtree (commands and pipes)
    struct ASTNode *right; // Right subtree (commands and pipes)
} LogicalNode;

// Define StartNode structure
typedef struct StartNode {
    LogicalNode **children; // Array of pointers to child nodes (logical AND/OR nodes)
    int childCount; // Number of children
    bool hasLogical; // Boolean to indicate if there are logical nodes
} StartNode;

void	init_terminal(char **env);
void	edit_shlvl(char **env);
void	write_terminal_title(void);
void	clear_terminal(char **env);
void  parser(Token *token);
void lexer(char *input);
void init_signal();
void start_terminal(int ac, char **av, char **env);
char *get_cwd();
char *clean_white_space(char *input);
// char *clean_command(char *input);
char *clean_quote(char *input);
char *clean_prompt(char *input);
void check_prompt(char *input);
char *clean_white_space(char *input);

#endif