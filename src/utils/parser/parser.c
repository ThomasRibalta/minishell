/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <jedurand@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 16:35:26 by jedurand          #+#    #+#             */
/*   Updated: 2024/06/29 23:44:21 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

t_astnode	*build_command_pipe_tree(t_token **current_token)
{
	t_astnode		*root;
	t_astnode		*current_command;
	t_redirections	redirs;

	root = NULL;
	current_command = NULL;
	redirs.inputs = NULL;
	redirs.outputs = NULL;
	while (*current_token != NULL)
	{
		if (process_token(current_token, &root, &current_command, &redirs))
			break ;
		*current_token = (*current_token)->next;
	}
	finalize_command(&root, &current_command, &redirs);
	return (root);
}

void	handle_outputs(t_redirection **temp_outputs, t_astnode *current_command)
{
	if (current_command)
		current_command->outputs = *temp_outputs;
	*temp_outputs = NULL;
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
			start_node->children[0]->left = build_command_pipe_tree(
					current_token);
		else if (*count == 0)
			start_node->children[0]->right = build_command_pipe_tree(
					current_token);
		else
			start_node->children[*count]->left = build_command_pipe_tree(
					current_token);
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
			generate_and_attach_btree_helper(start_node, &current_token,
				&count);
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
