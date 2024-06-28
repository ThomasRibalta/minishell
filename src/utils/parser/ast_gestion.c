/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_gestion.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thomas.rba <thomas.rba@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 10:06:16 by thomas.rba        #+#    #+#             */
/*   Updated: 2024/06/24 10:08:06 by thomas.rba       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

t_startnode	*create_start_node(void)
{
	t_startnode	*start_node;

	start_node = (t_startnode *)malloc(sizeof(t_startnode));
	if (!start_node)
	{
		perror("Failed to allocate memory for StartNode");
		exit(EXIT_FAILURE);
	}
	start_node->haslogical = false;
	start_node->childcount = 0;
	start_node->children = NULL;
	return (start_node);
}

t_startnode	*create_and_setup_start_node(t_token *tokens)
{
	t_startnode	*start_node;
	int			logical_node_count;

	start_node = create_start_node();
	logical_node_count = count_logical_nodes(tokens);
	setup_start_node(start_node, logical_node_count);
	return (start_node);
}

t_astnode	*create_ast_node(t_nodetype type, char *value)
{
	t_astnode	*node;

	node = (t_astnode *)malloc(sizeof(t_astnode));
	if (!node)
	{
		perror("Failed to allocate memory for ASTNode");
		exit(EXIT_FAILURE);
	}
	node->type = type;
	node->value = value;
	node->left = NULL;
	node->right = NULL;
	node->inputs = NULL;
	node->outputs = NULL;
	node->is_last_command = false;
	return (node);
}

t_logicalnode	*create_logical_node(t_nodetype type)
{
	t_logicalnode	*node;

	node = (t_logicalnode *)malloc(sizeof(t_logicalnode));
	if (!node)
	{
		perror("Failed to allocate memory for LogicalNode");
		exit(EXIT_FAILURE);
	}
	node->type = type;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_redirection	*create_redirection(char *filename, int characteristic)
{
	t_redirection	*redir;

	redir = (t_redirection *)malloc(sizeof(t_redirection));
	if (!redir)
	{
		perror("Failed to allocate memory for Redirection");
		exit(EXIT_FAILURE);
	}
	redir->filename = filename;
	redir->caracteristic = characteristic;
	redir->next = NULL;
	return (redir);
}
