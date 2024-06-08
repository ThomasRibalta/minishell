/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 14:43:51 by toto              #+#    #+#             */
/*   Updated: 2024/06/08 15:21:25 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../utils/gnl/get_next_line.h"
# include "../utils/libft/libft.h"
# include <dirent.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>
# define VIOLET_FONCE "\e[35;2m"
# define VIOLET "\e[35m"
# define VERT "\e[32m"
# define BLEU "\e[34m"
# define RESET "\e[0m"
# define MY_WEXITSTATUS(status) (((status)&0xff00) >> 8)

typedef enum t_tokentype
{
	TOKEN_PAREN,
	TOKEN_COMMAND,
	TOKEN_LOGICAL_AND,
	TOKEN_LOGICAL_OR,
	TOKEN_PIPE,
	TOKEN_HEREDOC,
	TOKEN_IN,
	TOKEN_OUT,
	TOKEN_APPEND,
}						t_tokentype;

typedef struct Command
{
	char				***env;
	char				***export;
	int					*pids;
	int					pid_count;
	int					std_in;
	int					std_out;
	int					fd;
	int					here_doc;
	int					p_id[2];
	int					p_id2[2];
}						t_command;

typedef struct token
{
	t_tokentype			type;
	char				*value;
	struct token		*next;
}						t_token;

typedef enum t_nodetype
{
	NODE_COMMAND,
	NODE_EMPTY_COMMAND,
	NODE_PARENTHESE,
	NODE_LOGICAL_AND,
	NODE_LOGICAL_OR,
	NODE_PIPE,
	NODE_LOGICAL_HOLDER,
}						t_nodetype;

typedef struct Redirection
{
	char				*filename;
	int					caracteristic;
	struct Redirection	*next;
}						t_redirection;

typedef struct ASTNode
{
	t_nodetype			type;
	char				*value;
	struct ASTNode		*left;
	struct ASTNode		*right;
	t_redirection		*here_doc;
	t_redirection		*inputs;
	t_redirection		*outputs;
	t_redirection		*appends;
	bool				builtin;
	bool				is_last_command;
}						t_astnode;

typedef struct LogicalNode
{
	t_nodetype			type;
	struct ASTNode		*left;
	struct ASTNode		*right;
}						t_logicalnode;

typedef struct StartNode
{
	t_logicalnode		**children;
	int					childcount;
	bool				haslogical;
}						t_startnode;

typedef struct Parsertmp
{
	t_astnode			*root;
	t_astnode			*last;
	t_astnode			*currentcommand;
	t_redirection		*tempinputs;
	t_redirection		*tempoutputs;
}						t_parsertmp;

extern int				g_global_sig;

int						len_tab(char **tab);
char					**check_wildcard(char **split_nodeValue);
void					init_terminal(char **env);
void					edit_shlvl(char **env);
void					write_terminal_title(void);
void					free_tab(char **tab);
void					clear_terminal(char **env);
void					parser(t_token *tokens, char ***env, char ***export,
							int *exit_status);
char					*get_env_value(char *word, char **env);
char					**fusionner_tableaux(char **tab1, char **tab2, int i);
char					**init_new_env(char ***env, int num_entries,
							const char *name, const char *value);
int						update_existing_entry(char **current, const char *name,
							const char *value);
int						error_name(const char *name);
char					*pwd(int i);
void					print_env(char **env, int nb);
void					exit_program(char **value);
int						export_var(char ***env, char ***export, char **tab);
char					**clone_env(char **env);
char					*get_path(char **env);
int						execute_builtin(t_command *cmd, char **param,
							t_astnode *node, int *exit_status);
int						is_builtin(char *value);
void					make_pipe(t_command *cmd, t_astnode *node);
int						is_fork_builtin(char *value);
int						execute_fork_builtin(char **env, char **export,
							char **param);
t_command				*init_command(int dupout, int dupin, char ***env,
							char ***export);
void					free_command(t_command *cmd, int *exit_status);
void					execute_output_append_command(t_astnode *node,
							t_command *cmd, int *exit_status);
void					execute_command(t_astnode *node, t_command *cmd,
							int *exit_status);
void					execute_output_append_parenthese(t_astnode *node,
							t_command *cmd, int *exit_status);
int						redirection_in(t_astnode *node, int *exit_status,
							t_command *cmd);
void					handle_child_process(t_astnode *node, t_command *cmd);
void					handle_parent_process(t_astnode *node, t_command *cmd);
int						open_output_append(t_astnode *node);
int						unset_var(char ***env, char **tab);
void					echo(char **tab);
int						cd(char **tab, char ***env);
void					lexer(char *input, char ***env, char ***export,
							int *exit_status);
void					init_signal(void);
void					start_terminal(char **env, char **export,
							int exit_status);
char					*get_cwd(int i);
char					*clean_white_space(char *input);
char					*clean_quote(char *input);
char					*clean_prompt(char *input, char ***env, char ***export,
							int *exit_status);
void					check_prompt(char *input, char ***env, char ***export,
							int *exit_status);
char					*clean_white_space(char *input);
void					replaceenvvars(char **str, char **env,
							int *exit_status);
void					free_lexer(t_token **lexer);
void					executer(t_startnode *startNode, char ***env,
							char ***export, int *exit_status);
void					execute(char **param, char *path, char **env);
void					printentireast(const t_startnode *startNode);
void					freeredirectionlist(t_redirection **list);
int						export_var2(char ***env, const char *name,
							const char *value);
void					expenser(t_startnode *startNode);
char					**clean_quote_all(char **split_nodeValue);
char					*remove_parenthese(char *str);
int						check_input_error(char *input);
bool					check_sequences(int i, int size, char **tab,
							char **symbols);
bool					contains_invalid_sequences(char **tab, int size,
							char **symbols);
int						ft_strcmp(char *value1, char *value2);
int						unset_var2(char ***env, const char *name);
t_token					*createtoken(t_tokentype type, const char *value);
void					appendtoken(t_token **head, t_tokentype type,
							const char *value);
void					freetokens(t_token *head);
char					**tab_clean(char **tab);
char					**init_symbols(void);
int						is_symbol(const char *str, int *symbol_length);
void					handle_symbol(const char *current, const char **start,
							char **result, int *count);
char					*fuse_split_result(char **split_input, int start);
int						count_tokens(const char *input);
char					*create_token(const char *start, const char *current);
char					*here_doc(char *end);
void					processtokens(t_startnode *startNode, t_token *tokens);
t_astnode				*buildcommand(t_token **cT);
void					type_command_or_parent(t_token **cT,
							t_astnode **currentCommand, t_astnode **root,
							t_astnode **last);
void					handlepipeorlogicaloperator(t_token **cT,
							t_parsertmp *parsertmp);
void					parser_redirection(t_redirection **tempInputs,
							t_redirection **tempOutputs, t_token **cT);
void					init_empty_cmd(t_parsertmp *parsertmp);
t_redirection			*createredirection(char *filename, int caracteristic);
t_logicalnode			*createlogicalNode(t_nodetype type);
t_astnode				*createastnode(t_nodetype type, char *value);
void					addredirection(t_redirection **list,
							t_redirection *redir);
t_startnode				*createandsetupstartnode(t_token *tokens);
char					*get_word_env(char *tmp, int i, int *j, char **env);
char					*get_env_value(char *word, char **env);
int						in_env(char *word, char **env);;
void					addlogicalnodetostartnode(t_startnode *startNode,
							t_token *tokens);
int						is_special_dir(const char *name);
int						count_all_files(DIR *dir);
int						count_files_with_suffix(DIR *dir, const char *suffix);
int						count_files_with_prefix(DIR *dir, const char *prefix);
int						count_files_with_prefix_and_suffix(DIR *dir,
							const char *prefix, const char *suffix);
int						get_number_of_files(char *after, char *before);
int						init_current_dir(int *n, int *i, char ***tab,
							DIR **dir);
char					**get_current_file(void);
char					**get_current_file_after(char *str);
char					**get_current_file_after_before(char *str, char *str2);
int						only_char(char *str, char c);

#endif
