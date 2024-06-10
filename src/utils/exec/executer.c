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

void	processbinarytree2(t_astnode *node, t_command *cmd)
{
	if (node == NULL)
		return ;
	processbinarytree2(node->left, cmd);
	if (node->type == NODE_COMMAND)
	{
		if (node->outputs || node->appends)
			execute_output_append_command(node, cmd);
		else
		{
			cmd->fd = -1;
			execute_command(node, cmd);
		}
	}
	if (node->type == NODE_PARENTHESE)
	{
		execute_output_append_parenthese(node, cmd);
	}
	processbinarytree2(node->right, cmd);
}

void	execute_logical_and_or(t_astnode *node, t_mainstruct mainstruct)
{
	t_command	*cmd;

	cmd = init_command(dup(STDOUT_FILENO), dup(STDIN_FILENO), mainstruct);
	processbinarytree2(node, cmd);
	free_command(cmd);
}

void	expandcommandtrees2(t_startnode *startnode, t_mainstruct mainstruct)
{
	int	i;

	i = -1;
	if (!startnode->haslogical)
		execute_logical_and_or(startnode->children[0]->left, mainstruct);
	else
	{
		while (++i < startnode->childcount)
		{
			if (startnode->children[i]->type == NODE_LOGICAL_AND)
			{
				if (startnode->children[i]->left && (*mainstruct.exit_status == 0
						|| i == 0))
					execute_logical_and_or(startnode->children[i]->left, mainstruct);
				if (i == 0 && startnode->children[i]->right
					&& *mainstruct.exit_status == 0)
					execute_logical_and_or(startnode->children[i]->right, mainstruct);
			}
			else if (startnode->children[i]->type == NODE_LOGICAL_OR)
			{
				if (startnode->children[i]->left && (*mainstruct.exit_status != 0
						|| i == 0))
					execute_logical_and_or(startnode->children[i]->left, mainstruct);
				if (i == 0 && startnode->children[i]->right
				&& *mainstruct.exit_status != 0)
					execute_logical_and_or(startnode->children[i]->right, mainstruct);
			}
		}
	}
}

void	executer(t_startnode *startnode, t_mainstruct mainstruct)
{
	g_global_sig = 1;
	expandcommandtrees2(startnode, mainstruct);
	g_global_sig = 0;
}
