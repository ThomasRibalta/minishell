/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thomas.rba <thomas.rba@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 10:10:56 by thomas.rba        #+#    #+#             */
/*   Updated: 2024/06/24 10:12:07 by thomas.rba       ###   ########.fr       */
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

void	process_redirection_token(t_token *current_token,
	t_redirection **temp_inputs, t_redirection **temp_outputs)
{
	t_redirection	*new_redir;

	new_redir = create_redirection(current_token->value,
			(current_token->type == TOKEN_APPEND
				|| current_token->type == TOKEN_HEREDOC));
	if (current_token->type == TOKEN_IN || current_token->type == TOKEN_HEREDOC)
		add_redirection(temp_inputs, new_redir);
	else if (current_token->type == TOKEN_OUT
		|| current_token->type == TOKEN_APPEND)
		add_redirection(temp_outputs, new_redir);
}

void	update_command_redirections(t_astnode **current_command,
	t_redirection *temp_inputs, t_redirection *temp_outputs)
{
	if (*current_command)
	{
		(*current_command)->inputs = temp_inputs;
		(*current_command)->outputs = temp_outputs;
	}
}

int	is_redirection(t_token *token)
{
	return (token->type == TOKEN_IN || token->type == TOKEN_OUT
		|| token->type == TOKEN_APPEND || token->type == TOKEN_HEREDOC);
}
