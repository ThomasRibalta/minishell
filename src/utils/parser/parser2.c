/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:46:08 by toto              #+#    #+#             */
/*   Updated: 2024/06/08 15:46:48 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

void	type_command_or_parent(t_token **ct, t_astnode **currentcommand,
		t_astnode **root, t_astnode **last)
{
	if ((*ct)->type == TOKEN_COMMAND)
		*currentcommand = createastnode(NODE_COMMAND, (*ct)->value);
	else
		*currentcommand = createastnode(NODE_PARENTHESE, (*ct)->value);
	if (*root == NULL)
		*root = *currentcommand;
	else
	{
		*last = *root;
		while ((*last)->right)
			*last = (*last)->right;
		(*last)->right = *currentcommand;
	}
}

void	init_empty_cmd(t_parsertmp *parsertmp)
{
	parsertmp->currentcommand = createastnode(NODE_EMPTY_COMMAND, NULL);
	(parsertmp->currentcommand)->inputs = parsertmp->tempinputs;
	(parsertmp->currentcommand)->outputs = parsertmp->tempoutputs;
	if (parsertmp->root == NULL)
		parsertmp->root = parsertmp->currentcommand;
	else
	{
		parsertmp->last = parsertmp->root;
		while ((parsertmp->last)->right)
			parsertmp->last = (parsertmp->last)->right;
		(parsertmp->last)->right = parsertmp->currentcommand;
	}
}

void	parser_redirection(t_redirection **tempinputs,
		t_redirection **tempoutputs, t_token **ct)
{
	t_redirection	*newredir;

	newredir = createredirection((*ct)->value, ((*ct)->type == TOKEN_APPEND
				|| (*ct)->type == TOKEN_HEREDOC));
	if ((*ct)->type == TOKEN_IN || (*ct)->type == TOKEN_HEREDOC)
		addredirection(tempinputs, newredir);
	else if ((*ct)->type == TOKEN_OUT || (*ct)->type == TOKEN_APPEND)
		addredirection(tempoutputs, newredir);
}

void	handlepipeorlogicaloperator(t_token **ct, t_parsertmp *parsertmp)
{
	t_astnode	*pipenode;

	if (!(parsertmp->currentcommand) && (parsertmp->tempinputs
			|| parsertmp->tempoutputs))
		init_empty_cmd(parsertmp);
	else
	{
		(parsertmp->currentcommand)->inputs = parsertmp->tempinputs;
		(parsertmp->currentcommand)->outputs = parsertmp->tempoutputs;
	}
	if ((*ct)->type == TOKEN_PIPE)
	{
		pipenode = createastnode(NODE_PIPE, "|");
		pipenode->left = parsertmp->root;
		parsertmp->root = pipenode;
		parsertmp->currentcommand = NULL;
	}
	else
	{
		parsertmp->tempinputs = NULL;
		parsertmp->tempoutputs = NULL;
	}
}
