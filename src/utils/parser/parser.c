/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thomas.rba <thomas.rba@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 16:35:26 by jedurand          #+#    #+#             */
/*   Updated: 2024/06/24 10:18:24 by thomas.rba       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

t_astnode	*build_command_pipe_tree_part1(t_token **current_token,
	t_astnode **root, t_astnode **current_command)
{
	while (*current_token != NULL)
	{
		if ((*current_token)->type == TOKEN_COMMAND
			|| (*current_token)->type == TOKEN_PAREN)
			process_command_token(root, current_command, *current_token);
		else if ((*current_token)->type == TOKEN_PIPE)
			handle_pipe_token(root, current_command);
		else
			break ;
		*current_token = (*current_token)->next;
	}
	return (*root);
}

t_astnode	*build_command_pipe_tree(t_token **current_token)
{
	t_astnode	*root;
	t_astnode	*current_command;

	root = NULL;
	current_command = NULL;
	root = build_command_pipe_tree_part1(current_token, &root,
			&current_command);
	return (build_command_pipe_tree_part2(current_token, &root,
			&current_command));
}

void	generate_and_attach_btree_helper(t_startnode *start_node,
	t_token **current_token, int *count)
{
	if ((*current_token)->type == TOKEN_LOGICAL_AND
		|| (*current_token)->type == TOKEN_LOGICAL_OR)
	{
		(*count)++;
		*current_token = (*current_token)->next;
	}
	else
	{
		if (*count == -1)
			start_node->children[0]->left
				= build_command_pipe_tree(current_token);
		else if (*count == 0)
			start_node->children[0]->right
				= build_command_pipe_tree(current_token);
		else
			start_node->children[*count]->left
				= build_command_pipe_tree(current_token);
	}
}

void	generate_and_attach_btree(t_startnode *start_node, t_token *tokens)
{
	t_token	*current_token;
	int		count;

	current_token = tokens;
	count = -1;
	if (!start_node->haslogical)
		start_node->children[0]->left = build_command_pipe_tree(&tokens);
	else
	{
		while (current_token != NULL)
		{
			generate_and_attach_btree_helper(start_node,
				&current_token, &count);
		}
	}
}

void	parser(t_token *tokens, t_mainstruct mainstruct)
{
	t_startnode	*start_node;

	start_node = create_and_setup_start_node(tokens);
	add_logical_node_to_start_node(start_node, tokens);
	generate_and_attach_btree(start_node, tokens);
	expenser(start_node);
	executer(start_node, mainstruct);
}
