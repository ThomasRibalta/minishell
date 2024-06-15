/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthese_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:16:45 by toto              #+#    #+#             */
/*   Updated: 2024/06/11 21:35:44 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

int	execute_builtin(t_command *cmd, char **param, t_astnode *node)
{
	if (ft_strcmp(clean_quote(param[0]), "exit") == 0 && node->is_last_command)
		exit_program(param + 1, cmd);
	else if (ft_strcmp(clean_quote(param[0]), "exit") == 0)
		clean_exit();
	else if ((ft_strcmp(clean_quote(param[0]), "cd") == 0
			|| ft_strcmp(clean_quote(param[0]), "cd") == 0)
		&& node->is_last_command)
		*cmd->mainstruct.exit_status = cd(param + 1, cmd->mainstruct.env,
				cmd->mainstruct.export);
	else if (ft_strcmp(clean_quote(param[0]), "export") == 0
		&& param[1] != NULL)
		*cmd->mainstruct.exit_status = export_var(cmd->mainstruct.env,
				cmd->mainstruct.export, param + 1);
	else if (ft_strcmp(clean_quote(param[0]), "unset") == 0)
		*cmd->mainstruct.exit_status = unset_var(cmd->mainstruct.env,
				cmd->mainstruct.export, param + 1);
	else
		return (1);
	free_tab(param);
	return (0);
}

int	execute_fork_builtin(char **env, char **export, char **par)
{
	char	*pwd2;

	if (ft_strcmp(clean_quote(par[0]), "echo") == 0
		|| ft_strcmp(clean_quote(par[0]), "/bin/echo") == 0)
		echo(par);
	else if (ft_strcmp(clean_quote(par[0]), "env") == 0
		|| ft_strcmp(clean_quote(par[0]), "/bin/env") == 0)
		print_env(env, 0);
	else if (ft_strcmp(clean_quote(par[0]), "export") == 0 && par[1] == NULL)
		print_env(export, 1);
	else if (ft_strcmp(clean_quote(par[0]), "cd") == 0
		|| ft_strcmp(clean_quote(par[0]), "cd") == 0)
		exit(0);
	else if (ft_strcmp(clean_quote(par[0]), "unset") == 0)
		exit(0);
	else if (ft_strcmp(clean_quote(par[0]), "pwd") == 0
		|| ft_strcmp(clean_quote(par[0]), "/bin/pwd") == 0)
	{
		pwd2 = pwd(0);
		free(pwd2);
	}
	else
		return (1);
	exit(0);
}

int	execute_parenthese2(t_command *cmd, t_astnode *node)
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
		here_doc = redirection_in(node, cmd->mainstruct.exit_status, cmd);
	if (here_doc == 2)
		return (2);
	return (here_doc);
}

void	execute_parenthese(t_astnode *node, t_command *cmd)
{
	pid_t	pid;
	char	*tmp;

	cmd->here_doc = execute_parenthese2(cmd, node);
	if (cmd->here_doc == 2)
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
		tmp = remove_parenthese(node->value);
		lexer(tmp, cmd->mainstruct);
		free(tmp);
		exit(EXIT_SUCCESS);
	}
	else
	{
		handle_parent_process(node, cmd);
		waitpid(pid, NULL, 0);
	}
}

void	execute_output_append_parenthese(t_astnode *node, t_command *cmd)
{
	int				i;
	t_redirection	*tmp;

	i = 0;
	if (node->outputs)
	{
		while (node->outputs->next)
		{
			tmp = node->outputs;
			node->outputs = node->outputs->next;
			free(tmp);
		}
		i++;
		cmd->fd = open_output_append(node);
		if (cmd->fd == -1)
			return ;
		execute_parenthese(node, cmd);
		close(cmd->fd);
	}
	if (!node->is_last_command || i == 0)
	{
		cmd->fd = -1;
		execute_parenthese(node, cmd);
	}
}
