/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:41:14 by toto              #+#    #+#             */
/*   Updated: 2024/06/11 16:24:53 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

t_redirection	*createredirection(char *filename, int caracteristic)
{
	t_redirection	*redir;

	redir = (t_redirection *)malloc(sizeof(t_redirection));
	if (!redir)
	{
		perror("Failed to allocate memory for Redirection");
		exit(EXIT_FAILURE);
	}
	redir->filename = filename;
	redir->caracteristic = caracteristic;
	redir->next = NULL;
	return (redir);
}

void	addredirection(t_redirection **list, t_redirection *redir)
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

void	generateandattachbtree(t_startnode *startnode, t_token *tokens)
{
	t_logicalnode	*holder;

	if (!startnode->haslogical)
	{
		holder = startnode->children[0];
		holder->left = buildcommand(&tokens);
	}
	else
	{
		processtokens(startnode, tokens);
	}
}

void	parser(t_token *tokens, t_mainstruct mainstruct)
{
	t_startnode	*startnode;

	(void)mainstruct;
	startnode = createandsetupstartnode(tokens);
	addlogicalnodetostartnode(startnode, tokens);
	generateandattachbtree(startnode, tokens);
	expenser(startnode);
	executer(startnode, mainstruct);
}
