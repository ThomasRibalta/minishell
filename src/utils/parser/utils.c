/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <jedurand@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 10:18:27 by thomas.rba        #+#    #+#             */
/*   Updated: 2024/06/29 23:45:39 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

int	count_logical_nodes(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current != NULL)
	{
		if (current->type == TOKEN_LOGICAL_AND
			|| current->type == TOKEN_LOGICAL_OR)
			count++;
		current = current->next;
	}
	return (count);
}

void	setup_start_node_children(t_startnode *start_node)
{
	start_node->children = (t_logicalnode **)malloc(sizeof(t_logicalnode *)
			* start_node->childcount);
	if (!start_node->children)
	{
		perror("Failed to allocate memory for logical node in StartNode");
		free(start_node);
		exit(EXIT_FAILURE);
	}
}

void	setup_start_node(t_startnode *start_node, int logical_node_count)
{
	if (logical_node_count > 0)
	{
		start_node->haslogical = true;
		start_node->childcount = logical_node_count;
	}
	else
		start_node->childcount = 1;
	setup_start_node_children(start_node);
}

void	finalize_command(t_astnode **root, t_astnode **cmd,
		t_redirections *redirs)
{
	if (!*cmd && (redirs->inputs || redirs->outputs))
		handle_empty_command(root, cmd, redirs);
	if (*cmd)
	{
		(*cmd)->inputs = redirs->inputs;
		(*cmd)->outputs = redirs->outputs;
	}
}
