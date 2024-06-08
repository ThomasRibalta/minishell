/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:22:02 by toto              #+#    #+#             */
/*   Updated: 2024/06/08 15:22:03 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

const char	*getnodetypestring(t_nodetype type)
{
	if (type == NODE_COMMAND)
		return ("COMMAND");
	else if (type == NODE_EMPTY_COMMAND)
		return ("EMPTY COMMAND");
	else if (type == NODE_PARENTHESE)
		return ("PARENTHESE");
	else if (type == NODE_LOGICAL_AND)
		return ("LOGICAL AND");
	else if (type == NODE_LOGICAL_OR)
		return ("LOGICAL OR");
	else if (type == NODE_PIPE)
		return ("PIPE");
	else if (type == NODE_LOGICAL_HOLDER)
		return ("LOGICAL HOLDER");
	else
		return ("UNKNOWN");
}

void	init_parsertmp(t_parsertmp *parsertmp)
{
	parsertmp->root = NULL;
	parsertmp->last = NULL;
	parsertmp->currentcommand = NULL;
	parsertmp->tempinputs = NULL;
	parsertmp->tempoutputs = NULL;
}

t_astnode	*buildcommand(t_token **ct)
{
	t_parsertmp	parsertmp;

	init_parsertmp(&parsertmp);
	while (*ct)
	{
		if ((*ct)->type == TOKEN_COMMAND || (*ct)->type == TOKEN_PAREN)
			type_command_or_parent(ct, &parsertmp.currentcommand,
				&parsertmp.root, &parsertmp.last);
		else if ((*ct)->type == TOKEN_PIPE || (*ct)->type == TOKEN_LOGICAL_AND
			|| (*ct)->type == TOKEN_LOGICAL_OR)
		{
			handlepipeorlogicaloperator(ct, &parsertmp);
			if ((*ct)->type != TOKEN_PIPE)
				return (parsertmp.root);
		}
		else if ((*ct)->type == TOKEN_IN || (*ct)->type == TOKEN_OUT
			|| (*ct)->type == TOKEN_APPEND || (*ct)->type == TOKEN_HEREDOC)
			parser_redirection(&parsertmp.tempinputs, &parsertmp.tempoutputs,
				ct);
		*ct = (*ct)->next;
	}
	if (parsertmp.currentcommand == NULL && (parsertmp.tempinputs
			|| parsertmp.tempoutputs))
		init_empty_cmd(&parsertmp);
	else if (parsertmp.currentcommand)
	{
		parsertmp.currentcommand->inputs = parsertmp.tempinputs;
		parsertmp.currentcommand->outputs = parsertmp.tempoutputs;
	}
	return (parsertmp.root);
}

void	processtokens(t_startnode *startnode, t_token *tokens)
{
	int	count;

	count = -1;
	while (tokens != NULL)
	{
		if (tokens->type == TOKEN_LOGICAL_AND
			|| tokens->type == TOKEN_LOGICAL_OR)
		{
			count++;
			tokens = tokens->next;
		}
		else
		{
			if (count == -1)
				startnode->children[0]->left = buildcommand(&tokens);
			else if (count == 0)
				startnode->children[0]->right = buildcommand(&tokens);
			else
				startnode->children[count]->left = buildcommand(&tokens);
		}
	}
}
