/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:26:23 by toto              #+#    #+#             */
/*   Updated: 2024/06/11 16:28:55 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

int	countlogicalnodes(t_token *tokens)
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

t_startnode	*createandsetupstartnode(t_token *tokens)
{
	int			logicalnodecount;
	t_startnode	*startnode;

	startnode = (t_startnode *)malloc(sizeof(t_startnode));
	if (!startnode)
		exit(EXIT_FAILURE);
	startnode->haslogical = false;
	startnode->childcount = 0;
	startnode->children = NULL;
	logicalnodecount = countlogicalnodes(tokens);
	if (logicalnodecount > 0)
	{
		startnode->haslogical = true;
		startnode->childcount = logicalnodecount;
	}
	else
		startnode->childcount = 1;
	startnode->children = (t_logicalnode **)malloc(sizeof(t_logicalnode *)
			* startnode->childcount);
	if (!startnode->children)
	{
		free(startnode);
		exit(EXIT_FAILURE);
	}
	return (startnode);
}

t_astnode	*createastnode(t_nodetype type, char *value)
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
	node->builtin = false;
	node->is_last_command = false;
	return (node);
}

t_logicalnode	*createlogicalnode(t_nodetype type)
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
