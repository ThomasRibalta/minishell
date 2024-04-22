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
# include "../utils/parser/parser.h"
# define VIOLET_FONCE "\e[35;2m"
# define VIOLET "\e[35m"
# define VERT "\e[32m"
# define BLEU "\e[34m"
# define RESET "\e[0m"

// typedef enum {
//     TOKEN_PAREN,
//     TOKEN_COMMAND,
//     TOKEN_LOGICAL_AND,
//     TOKEN_LOGICAL_OR,
//     TOKEN_PIPE,
// 	TOKEN_HEREDOC,
//     TOKEN_REDIRECTION_IN,
//     TOKEN_REDIRECTION_OUT,
//     TOKEN_REDIRECTION_APPEND,
//     TOKEN_WILDCARD,
// } TokenType;

// typedef struct Token {
//     TokenType type;
//     char* value;
//     struct Token* next;
// } Token;

void	init_terminal(char **env);
void	edit_shlvl(char **env);
void	write_terminal_title(void);
void	clear_terminal(char **env);
void  parser(Token *tokens);
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