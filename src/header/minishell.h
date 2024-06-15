/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 14:43:51 by toto              #+#    #+#             */
/*   Updated: 2024/06/11 21:13:55 by toto             ###   ########.fr       */
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
# include <fcntl.h>
# define VIOLET_FONCE "\e[35;2m"
# define VIOLET "\e[35m"
# define VERT "\e[32m"
# define BLEU "\e[34m"
# define RESET "\e[0m"

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
	t_redirection		*inputs;
	t_redirection		*outputs;
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

typedef struct MainStruct
{
	char				***env;
	char				***export;
	int					*exit_status;
	int					*exit;
}						t_mainstruct;

typedef struct Command
{
	int					*pids;
	int					pid_count;
	int					std_in;
	int					std_out;
	int					fd;
	int					here_doc;
	int					p_id[2];
	int					p_id2[2];
	t_mainstruct		mainstruct;
}						t_command;

extern int				g_global_sig;

void					free_tab(char **tab);
char					*clean_white_space(char *input);
char					*clean_quote(char *input);
void					clean_prompt(char *input, t_mainstruct mainstruct);
void					check_prompt(char *input, t_mainstruct mainstruct);
char					*remove_parenthese(char *str);
char					**clean_quote_all(char **split_nodeValue);
int						len_tab(char **tab);
void					start_terminal(char **env, char **export,
							int exit_status);
void					init_terminal(char **env);
void					clean_exit(void);
char					**clone_env(char **env);
void					write_terminal_title(void);
char					**get_current_file(void);
char					**get_current_file_after(char *str);
char					**get_current_file_after_before(char *str, char *str2);
int						only_char(char *str, char c);
int						init_current_dir(int *n, int *i, char ***tab,
							DIR **dir);
int						get_number_of_files(char *after, char *before);
int						count_files_with_prefix_and_suffix(DIR *dir,
							const char *prefix, const char *suffix);
int						count_files_with_prefix(DIR *dir, const char *prefix);
char					**fusionner_tableaux(char **tab1, char **tab2, int i);
int						is_special_dir(const char *name);
int						count_all_files(DIR *dir);
int						count_files_with_suffix(DIR *dir, const char *suffix);
char					**check_full_wildcard(char **tab_current_file, int i);
char					**check_leading_wildcard(char **tab_current_file,
							int i);
char					**check_middle_wildcard(char **tab_current_file, int i,
							int j);
char					**check_wildcard(char **split_nodeValue);
char					*replacesubstring(char *str, int start, int end,
							char *replacement);
void					replace_variable(char **tmp, int *i, char **env,
							int *exit_status);
void					replaceenvvars(char **str, char **env,
							int *exit_status);
void					edit_shlvl(char **env);
void					addlogicalnodetostartnode(t_startnode *startnode,
							t_token *tokens);
int						in_env(char *word, char **env);
char					*get_env_value(char *word, char **env);
char					*get_word_env(char *tmp, int i, int *j, char **env);
const char				*getnodetypestring(t_nodetype type);
void					init_parsertmp(t_parsertmp *parsertmp);
t_astnode				*buildcommand(t_token **ct);
void					processtokens(t_startnode *startnode, t_token *tokens);
int						countlogicalnodes(t_token *tokens);
t_startnode				*createandsetupstartnode(t_token *tokens);
t_astnode				*createastnode(t_nodetype type, char *value);
t_logicalnode			*createlogicalnode(t_nodetype type);
void					type_command_or_parent(t_token **ct,
							t_astnode **currentcommand, t_astnode **root,
							t_astnode **last);
void					init_empty_cmd(t_parsertmp *parsertmp);
void					parser_redirection(t_redirection **tempinputs,
							t_redirection **tempoutputs, t_token **ct);
void					handlepipeorlogicaloperator(t_token **ct,
							t_parsertmp *parsertmp);
t_redirection			*createredirection(char *filename, int caracteristic);
void					addredirection(t_redirection **list,
							t_redirection *redir);
void					generateandattachbtree(t_startnode *startnode,
							t_token *tokens);
void					parser(t_token *tokens, t_mainstruct mainstruct);
void					is_last_command_btree(t_astnode *node);
void					is_last_command(t_startnode *startNode);
void					expenser(t_startnode *startNode);
t_token					*createtoken(t_tokentype type, char *value);
void					appendtoken(t_token **head, t_tokentype type,
							char *value);
void					freetokens(t_token *head);
char					**init_symbols(void);
int						is_symbol(const char *str, int *symbol_length);
void					handle_symbol(const char *current, const char **start,
							char **result, int *count);
char					*fuse_split_result(char **split_input, int start);
int						count_tokens(const char *input);
char					*create_token(const char *start, const char *current);
char					*here_doc(char *end);
char					**split_with_symbols(const char *input);
void					chained_split_prompt(t_token **list, char **tab);
char					**re_build_with_redir(char **tab_input);
void					lexer(char *input, t_mainstruct mainstruct);
char					**tab_clean(char **tab);
int						check_input_error(char *input);
bool					check_sequences(int i, int size, char **tab,
							char **symbols);
bool					contains_invalid_sequences(char **tab, int size,
							char **symbols);
int						is_builtin(char *value);
int						is_fork_builtin(char *value);
void					make_pipe(t_command *cmd, t_astnode *node);
t_command				*init_command(int dupout, int dupin,
							t_mainstruct mainstruct);
void					free_command(t_command *cmd);
int						redirection_in(t_astnode *node, int *exit_status,
							t_command *cmd);
void					handle_child_process(t_astnode *node, t_command *cmd);
void					handle_parent_process(t_astnode *node, t_command *cmd);
int						open_output_append(t_astnode *node);
int						execute_builtin(t_command *cmd, char **param,
							t_astnode *node);
int						execute_fork_builtin(char **env, char **export,
							char **param);
int						execute_parenthese2(t_command *cmd, t_astnode *node);
void					execute_parenthese(t_astnode *node, t_command *cmd);
void					execute_output_append_parenthese(t_astnode *node,
							t_command *cmd);
char					**execute_commande_split(t_astnode *node,
							int *exit_status, int *here_doc, t_command *cmd);
void					execute_command2(t_astnode *node, t_command *cmd,
							char **split_nodevalue, pid_t pid);
void					execute_command(t_astnode *node, t_command *cmd);
void					execute_output_append_command(t_astnode *node,
							t_command *cmd);
char					*get_path(char **env);
char					*check_path(char **paths, char *name);
void					execute(char **param, char *path, char **env);
void					processbinarytree2(t_astnode *node, t_command *cmd);
void					execute_logical_and_or(t_astnode *node,
							t_mainstruct mainstruct);
void					expandcommandtrees2(t_startnode *startnode,
							t_mainstruct mainstruct);
void					executer(t_startnode *startnode,
							t_mainstruct mainstruct);
int						cd(char **tab, char ***env, char ***export);
void					echo(char **tab);
void					print_env(char **env, int nb);
void					exit_program(char **value, t_command *cmd);
int						error_name(const char *name);
int						update_existing_entry(char **current, char *name,
							char *value);
char					**duplicate_env(char **env);
void					init_new_env(char ***env, int num_entries, char *name,
							char *value);
int						add_new_entry(char ***env, char *name, char *value);
int						export_var2(char ***env, char *name, char *value);
int						export_var(char ***env, char ***export, char **tab);
char					*get_cwd(int i);
char					*pwd(int i);
int						unset_var2(char ***env, const char *name);
int						unset_var(char ***env, char ***export, char **tab);
void					init_signal(void);
void					free_parser(t_startnode *startnode);
t_mainstruct			intialize_mainstruct(char ***env, char ***export,
							int *exit_status, int *exit);
int						my_wexitstatus(int status);
void					aff_tab(char **tab);
void					stop_process(t_mainstruct mainstruct,
							char *make_readline);

#endif
