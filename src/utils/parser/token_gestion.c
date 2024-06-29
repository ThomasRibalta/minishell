/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_gestion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <jedurand@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 10:15:10 by thomas.rba        #+#    #+#             */
/*   Updated: 2024/06/29 23:44:43 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

void	add_logical_node(t_startnode *start_node, t_token *current_token,
		int *index)
{
	t_nodetype	type;

	if (current_token->type == TOKEN_LOGICAL_AND)
		type = NODE_LOGICAL_AND;
	else if (current_token->type == TOKEN_LOGICAL_OR)
		type = NODE_LOGICAL_OR;
	else
		return ;
	start_node->children[*index] = create_logical_node(type);
	(*index)++;
}

void	add_logical_node_to_start_node(t_startnode *start_node, t_token *tokens)
{
	int		index;
	t_token	*current_token;

	index = 0;
	current_token = tokens;
	if (start_node->haslogical)
	{
		while (current_token != NULL && index < start_node->childcount)
		{
			add_logical_node(start_node, current_token, &index);
			current_token = current_token->next;
		}
	}
	else
		start_node->children[0] = create_logical_node(NODE_LOGICAL_HOLDER);
}

void	process_other_tokens(t_astnode **root, t_astnode **current_command)
{
	t_astnode	*last;

	if (!*current_command)
	{
		*current_command = create_ast_node(NODE_EMPTY_COMMAND, NULL);
		if (*root == NULL)
		{
			*root = *current_command;
		}
		else
		{
			last = *root;
			while (last->right != NULL)
				last = last->right;
			last->right = *current_command;
			(*current_command)->inputs = NULL;
			(*current_command)->outputs = NULL;
		}
	}
}

int	process_token(t_token **token, t_astnode **root, t_astnode **cmd,
		t_redirections *redirs)
{
	if ((*token)->type == TOKEN_COMMAND || (*token)->type == TOKEN_PAREN)
		process_command_token(root, cmd, *token);
	else if ((*token)->type == TOKEN_PIPE)
		handle_pipe_token(root, cmd, redirs);
	else if (is_redirection(*token))
		process_redirection_token(*token, redirs);
	else if (is_logical_operator(*token))
		return (1);
	return (0);
}
