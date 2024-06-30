/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <jedurand@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 10:10:56 by thomas.rba        #+#    #+#             */
/*   Updated: 2024/06/29 23:45:19 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

void	add_redirection(t_redirection **list, t_redirection *redir)
{
	t_redirection	*current;

	if (*list == NULL)
		*list = redir;
	else
	{
		current = *list;
		while (current->next != NULL)
			current = current->next;
		current->next = redir;
	}
}

void	update_command_redirections(t_astnode **current_command,
		t_redirections *redirs)
{
	if (*current_command)
	{
		(*current_command)->inputs = redirs->inputs;
		(*current_command)->outputs = redirs->outputs;
	}
}

int	is_redirection(t_token *token)
{
	if (token->type == TOKEN_IN || token->type == TOKEN_OUT)
		return (1);
	if (token->type == TOKEN_APPEND || token->type == TOKEN_HEREDOC)
		return (1);
	return (0);
}

int	is_logical_operator(t_token *token)
{
	if (token->type == TOKEN_LOGICAL_AND)
		return (1);
	if (token->type == TOKEN_LOGICAL_OR)
		return (1);
	return (0);
}
