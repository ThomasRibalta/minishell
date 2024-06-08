/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 14:43:47 by toto              #+#    #+#             */
/*   Updated: 2024/06/08 14:43:49 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

t_token	*createt_token(t_tokentype type, const char *value)
{
	t_token	*tokens;

	tokens = (t_token *)malloc(sizeof(t_token));
	tokens->type = type;
	if (value != NULL)
		tokens->value = strdup(value);
	else
		tokens->value = NULL;
	tokens->next = NULL;
	return (tokens);
}

void	appendt_token(t_token **head, t_tokentype type, const char *value)
{
	t_token	*newtoken;
	t_token	*current;

	newtoken = createtoken(type, value);
	if (*head == NULL)
	{
		*head = newtoken;
	}
	else
	{
		current = *head;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = newtoken;
	}
}

void	freetokens(t_token *head)
{
	t_token	*tmp;

	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		if (tmp->value != NULL)
			free(tmp->value);
		free(tmp);
	}
}
