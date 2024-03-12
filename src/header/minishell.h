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
# define VIOLET_FONCE "\e[35;2m"
# define VIOLET "\e[35m"
# define VERT "\e[32m"
# define BLEU "\e[34m"
# define RESET "\e[0m"

void	init_terminal(char **env);
void	edit_shlvl(char **env);
void	write_terminal_title(void);
void	clear_terminal(char **env);
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