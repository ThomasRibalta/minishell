#ifndef MINISHELL_H
# define MINISHELL_H

# include "../utils/libft/libft.h"
# include "../utils/gnl/get_next_line.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <signal.h>
# include <dirent.h>
# include <sys/wait.h>
# include <unistd.h>
# include <stdbool.h>
# define VIOLET_FONCE "\e[35;2m"
# define VIOLET "\e[35m"
# define VERT "\e[32m"
# define BLEU "\e[34m"
# define RESET "\e[0m"
# define MY_WEXITSTATUS(status) (((status) & 0xff00) >> 8)

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

typedef struct Command {
    int *pids;
    int pid_count;
    int std_in;
    int std_out;
} command;

typedef struct Token {
    TokenType type;
    char* value;
    struct Token* next;
} Token;

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
    int caracteristic;
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
	bool is_last_command;
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

extern int global_sig;


int len_tab(char **tab);
char **check_wildcard(char **split_nodeValue);
void	init_terminal(char **env);
void	edit_shlvl(char **env);
void	write_terminal_title(void);
void free_tab(char **tab);
void	clear_terminal(char **env);
void  parser(Token *tokens, char ***env, char ***export, int *exit_status);
char *get_env_value(char *word, char **env , int *exit_status);
char **fusionner_tableaux(char **tab1, char **tab2, int sizetab1, int sizetab2, int i);
int error_name(const char *name);
char *pwd(int i);
void print_env(char **env, int nb);
void exit_program(char **value);
int export_var(char ***env, char***export, char **tab);
char **clone_env(char **env);
int unset_var(char ***env, char **tab);
void echo(char **tab);
int cd(char **tab, char ***env);
void lexer(char *input, char ***env, char ***export, int *exit_status);
void init_signal();
void start_terminal(char **env, char **export, int exit_status);
char *get_cwd(int i);
char *clean_white_space(char *input);
char *clean_quote(char *input);
char *clean_prompt(char *input, char ***env, char ***export, int *exit_status);
void check_prompt(char *input, char ***env, char ***export, int *exit_status);
char *clean_white_space(char *input);
void replaceEnvVars(char **str, char **env, int *exit_status);
void	free_lexer(Token **lexer);
void executer(StartNode* startNode, char ***env, char ***export, int *exit_status);
void printEntireAST(const StartNode* startNode);
void freeRedirectionList(Redirection** list);
int export_var2(char ***env, const char *name, const char *value);
void expenser(StartNode* startNode);
char **clean_quote_all(char **split_nodeValue);
int unset_var2(char ***env, const char *name);


#endif
