/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 21:01:59 by toto              #+#    #+#             */
/*   Updated: 2024/06/11 21:33:10 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

void	processbinarytree2(t_astnode *node, t_command *cmd)
{
	if (node == NULL)
		return ;
	processbinarytree2(node->left, cmd);
	if (node->type == NODE_EMPTY_COMMAND)
		exit(0);
	if (node->type == NODE_COMMAND && *cmd->mainstruct.exit == -1)
	{
		if (node->outputs)
			execute_output_append_command(node, cmd);
		else
		{
			cmd->fd = -1;
			execute_command(node, cmd);
		}
	}
	if (node->type == NODE_PARENTHESE && *cmd->mainstruct.exit == -1)
	{
		execute_output_append_parenthese(node, cmd);
	}
	processbinarytree2(node->right, cmd);
}

void	free_node(t_astnode *node)
{
	t_redirection	*tmp;

	if (node->inputs)
	{
		while (node->inputs)
		{
			tmp = node->inputs;
			node->inputs = node->inputs->next;
			free(tmp);
		}
	}
	if (node->outputs)
	{
		while (node->outputs)
		{
			tmp = node->outputs;
			node->outputs = node->outputs->next;
			free(tmp);
		}
	}
	free(node);
}

void	free_ast(t_astnode *node)
{
	if (node == NULL)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	free_node(node);
}

void	free_parser(t_startnode *startnode)
{
	int	i;

	i = -1;
	if (!startnode->haslogical)
	{
		free_ast(startnode->children[0]->left);
		free(startnode->children[0]);
	}
	else
	{
		while (++i < startnode->childcount)
		{
			if (startnode->children[i]->left)
				free_ast(startnode->children[i]->left);
			if (startnode->children[i]->right)
				free_ast(startnode->children[i]->right);
			free(startnode->children[i]);
		}
	}
}
