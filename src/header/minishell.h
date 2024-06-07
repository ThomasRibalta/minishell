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
    TOKEN_IN,
    TOKEN_OUT,
    TOKEN_APPEND,
} TokenType;

typedef struct Command {
    char ***env;
    char ***export;
    int *pids;
    int pid_count;
    int std_in;
    int std_out;
    int fd;
    int here_doc;
    int p_id[2];
    int p_id2[2];
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

typedef struct Parsertmp {
    ASTNode* root;
    ASTNode* last;
    ASTNode* currentCommand;
    Redirection *tempInputs;
    Redirection *tempOutputs;
} Parsertmp;

extern int global_sig;


int len_tab(char **tab);
char **check_wildcard(char **split_nodeValue);
void	init_terminal(char **env);
void	edit_shlvl(char **env);
void	write_terminal_title(void);
void free_tab(char **tab);
void	clear_terminal(char **env);
void  parser(Token *tokens, char ***env, char ***export, int *exit_status);
char *get_env_value(char *word, char **env);
char **fusionner_tableaux(char **tab1, char **tab2, int i);
char **init_new_env(char ***env, int num_entries, const char *name, const char *value);
int update_existing_entry(char **current, const char *name, const char *value);
int error_name(const char *name);
char *pwd(int i);
void print_env(char **env, int nb);
void exit_program(char **value);
int export_var(char ***env, char***export, char **tab);
char **clone_env(char **env);
char *get_path(char **env);
int execute_builtin(command *cmd, char **param, ASTNode *node, int *exit_status);
int is_builtin(char *value);
void make_pipe(command *cmd, ASTNode *node);
int is_fork_builtin(char *value);
int execute_fork_builtin(char **env, char **export, char **param);
command *init_command(int dupout, int dupin, char ***env, char ***export);
void    free_command(command *cmd, int *exit_status);
void execute_output_append_command(ASTNode* node, command *cmd, int *exit_status);
void execute_command(ASTNode* node, command *cmd, int *exit_status);
void execute_output_append_parenthese(ASTNode* node, command *cmd, int *exit_status);
int redirection_in(ASTNode *node, int *exit_status, command *cmd);
void handle_child_process(ASTNode *node, command *cmd);
void handle_parent_process(ASTNode *node, command *cmd);
int open_output_append(ASTNode *node);
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
void execute(char **param, char *path, char **env);
void printEntireAST(const StartNode* startNode);
void freeRedirectionList(Redirection** list);
int export_var2(char ***env, const char *name, const char *value);
void expenser(StartNode* startNode);
char **clean_quote_all(char **split_nodeValue);
char *remove_parenthese(char *str);
int check_input_error(char *input);
bool check_sequences(int i, int size, char **tab, char **symbols);
bool contains_invalid_sequences(char **tab, int size, char **symbols);
int ft_strcmp(char *value1, char *value2);
int unset_var2(char ***env, const char *name);
Token* createToken(TokenType type, const char* value);
void appendToken(Token** head, TokenType type, const char* value);
void freeTokens(Token* head);
char **tab_clean(char **tab);
char **init_symbols();
int is_symbol(const char *str, int *symbol_length);
void handle_symbol(const char *current, const char **start, char **result, int *count);
char *fuse_split_result(char **split_input, int start);
int count_tokens(const char *input);
char *create_token(const char *start, const char *current);
char *here_doc(char *end);
void processTokens(StartNode* startNode, Token* tokens);
ASTNode* buildCommandPipeTree(Token** cT);
void type_command_or_parent(Token **cT, ASTNode **currentCommand, ASTNode **root, ASTNode **last);
void handlePipeOrLogicalOperator(Token** cT, Parsertmp *parsertmp);
void parser_redirection(Redirection **tempInputs, Redirection **tempOutputs, Token **cT);
void init_empty_cmd(Parsertmp *parsertmp);
Redirection* createRedirection(char* filename, int caracteristic);
LogicalNode* createLogicalNode(NodeType type);
ASTNode* createASTNode(NodeType type, char* value);
void addRedirection(Redirection** list, Redirection* redir);
StartNode* createAndSetupStartNode(Token* tokens);
char *get_word_env(char *tmp, int i, int *j, char **env);
char *get_env_value(char *word, char **env);
int in_env(char *word, char **env);
void replaceEnvVars(char **str, char **env, int *exit_status);
void addLogicalNodeToStartNode(StartNode* startNode, Token* tokens);
int	is_special_dir(const char *name);
int	count_all_files(DIR *dir);
int	count_files_with_suffix(DIR *dir, const char *suffix);
int	count_files_with_prefix(DIR *dir, const char *prefix);
int	count_files_with_prefix_and_suffix(DIR *dir, const char *prefix, const char *suffix);
int	get_number_of_files(char *after, char *before);
int init_current_dir(int *n, int *i, char ***tab, DIR **dir);
char **get_current_file();
char **get_current_file_after(char *str);
char **get_current_file_after_before(char *str, char *str2);
int only_char(char *str, char c);

#endif
