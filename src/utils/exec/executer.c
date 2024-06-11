/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:06:19 by toto              #+#    #+#             */
/*   Updated: 2024/06/11 21:09:55 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

void	execute_logical_and_or(t_astnode *node, t_mainstruct mainstruct)
{
	t_command	*cmd;

	cmd = init_command(dup(STDOUT_FILENO), dup(STDIN_FILENO), mainstruct);
	processbinarytree2(node, cmd);
	free_command(cmd);
}

void	handle_logical_node(t_startnode *startnode, int i,
		t_mainstruct mainstruct)
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

void	expandcommandtrees2(t_startnode *startnode, t_mainstruct mainstruct)
{
	int	i;

	i = -1;
	if (!startnode->haslogical)
	{
		execute_logical_and_or(startnode->children[0]->left, mainstruct);
	}
	else
	{
		while (++i < startnode->childcount)
		{
			handle_logical_node(startnode, i, mainstruct);
		}
	}
}

void	executer(t_startnode *startnode, t_mainstruct mainstruct)
{
	g_global_sig = 1;
	expandcommandtrees2(startnode, mainstruct);
	free_parser(startnode);
	free(startnode->children);
	free(startnode);
	g_global_sig = 0;
}
