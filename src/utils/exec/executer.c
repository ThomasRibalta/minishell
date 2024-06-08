/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:06:19 by toto              #+#    #+#             */
/*   Updated: 2024/06/08 16:07:23 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

void	processbinarytree2(t_astnode *node, t_command *cmd, int *exit_status)
{
	if (node == NULL)
		return ;
	processbinarytree2(node->left, cmd, exit_status);
	if (node->type == NODE_COMMAND)
	{
		if (node->outputs || node->appends)
			execute_output_append_command(node, cmd, exit_status);
		else
		{
			cmd->fd = -1;
			execute_command(node, cmd, exit_status);
		}
	}
	if (node->type == NODE_PARENTHESE)
	{
		execute_output_append_parenthese(node, cmd, exit_status);
	}
	processbinarytree2(node->right, cmd, exit_status);
}

void	execute_logical_and_or(t_astnode *node, char ***env, char ***export,
		int *exit_status)
{
	t_command	*cmd;

	cmd = init_command(dup(STDOUT_FILENO), dup(STDIN_FILENO), env, export);
	processbinarytree2(node, cmd, exit_status);
	free_command(cmd, exit_status);
}

void	expandcommandtrees2(t_startnode *startnode, char ***env, char ***export,
		int *exit_status)
{
	int	i;

	i = -1;
	if (!startnode->haslogical)
		execute_logical_and_or(startnode->children[0]->left, env, export,
			exit_status);
	else
	{
		while (++i < startnode->childcount)
		{
			if (startnode->children[i]->type == NODE_LOGICAL_AND)
			{
				if (startnode->children[i]->left && (*exit_status == 0
						|| i == 0))
					execute_logical_and_or(startnode->children[i]->left, env,
						export, exit_status);
				if (i == 0 && startnode->children[i]->right
					&& *exit_status == 0)
					execute_logical_and_or(startnode->children[i]->right, env,
						export, exit_status);
			}
			else if (startnode->children[i]->type == NODE_LOGICAL_OR)
			{
				if (startnode->children[i]->left && (*exit_status != 0
						|| i == 0))
					execute_logical_and_or(startnode->children[i]->left, env,
						export, exit_status);
				if (i == 0 && startnode->children[i]->right
					&& *exit_status != 0)
					execute_logical_and_or(startnode->children[i]->right, env,
						export, exit_status);
			}
		}
	}
}

void	executer(t_startnode *startnode, char ***env, char ***export,
		int *exit_status)
{
	g_global_sig = 1;
	expandcommandtrees2(startnode, env, export, exit_status);
	g_global_sig = 0;
}
