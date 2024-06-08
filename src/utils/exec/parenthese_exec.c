/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthese_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:16:45 by toto              #+#    #+#             */
/*   Updated: 2024/06/08 16:21:56 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

int	execute_builtin(t_command *cmd, char **param, t_astnode *node,
	int *exit_status)
{
	if (ft_strcmp(clean_quote(param[0]), "exit") == 0 && node->is_last_command)
		exit_program(param + 1);
	else if ((ft_strcmp(clean_quote(param[0]), "cd") == 0
			|| ft_strcmp(clean_quote(param[0]), "cd") == 0)
		&& node->is_last_command)
		*exit_status = cd(param + 1, cmd->env);
	else if (ft_strcmp(clean_quote(param[0]), "export") == 0
		&& param[1] != NULL)
		*exit_status = export_var(cmd->env, cmd->export, param + 1);
	else if (ft_strcmp(clean_quote(param[0]), "unset") == 0)
		*exit_status = unset_var(cmd->env, param + 1);
	else
		return (1);
	return (0);
}

int	execute_fork_builtin(char **env, char **export, char **param)
{
	if (ft_strcmp(clean_quote(param[0]), "echo") == 0
		|| ft_strcmp(clean_quote(param[0]), "/bin/echo") == 0)
		echo(param);
	else if (ft_strcmp(clean_quote(param[0]), "env") == 0
		|| ft_strcmp(clean_quote(param[0]), "/bin/env") == 0)
		print_env(env, 0);
	else if (ft_strcmp(clean_quote(param[0]), "export") == 0
		&& param[1] == NULL)
		print_env(export, 1);
	else if (ft_strcmp(clean_quote(param[0]), "cd") == 0
		|| ft_strcmp(clean_quote(param[0]), "cd") == 0)
		exit(0);
	else if (ft_strcmp(clean_quote(param[0]), "unset") == 0)
		exit(0);
	else if (ft_strcmp(clean_quote(param[0]), "pwd") == 0
		|| ft_strcmp(clean_quote(param[0]), "/bin/pwd") == 0)
		printf("%s\n", pwd(0));
	else
		return (1);
	exit(0);
}

int	execute_parenthese2(t_command *cmd, t_astnode *node, int *exit_status)
{
	int	here_doc;

	here_doc = 0;
	if (pipe(cmd->p_id) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	if (cmd->fd == -1)
		cmd->fd = cmd->p_id[1];
	if (node->inputs)
		here_doc = redirection_in(node, exit_status, cmd);
	if (here_doc == 2)
		return (0);
	return (here_doc);
}

void	execute_parenthese(t_astnode *node, t_command *cmd, int *exit_status)
{
	pid_t	pid;

	cmd->here_doc = execute_parenthese2(cmd, node, exit_status);
	if (cmd->here_doc == 0)
		return ;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		handle_child_process(node, cmd);
		lexer(remove_parenthese(node->value), cmd->env, cmd->export,
			exit_status);
		exit(EXIT_SUCCESS);
	}
	else
	{
		handle_parent_process(node, cmd);
		waitpid(pid, NULL, 0);
	}
}

void	execute_output_append_parenthese(t_astnode *node, t_command *cmd,
		int *exit_status)
{
	int	i;

	i = 0;
	if (node->outputs)
	{
		i++;
		cmd->fd = open_output_append(node);
		if (cmd->fd == -1)
			return ;
		execute_parenthese(node, cmd, exit_status);
		close(cmd->fd);
	}
	if (!node->is_last_command || i == 0)
	{
		cmd->fd = -1;
		execute_parenthese(node, cmd, exit_status);
	}
}
