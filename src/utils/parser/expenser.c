/** ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expenser.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 14:47:06 by toto              #+#    #+#             */
/*   Updated: 2024/06/08 14:47:07 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

void	is_last_command_btree(t_astnode *node)
{
	if (node == NULL)
		return ;
	while (node->right != NULL)
	{
		node = node->right;
	}
	node->is_last_command = true;
}

void	is_last_command(t_startnode *startNode)
{
	int	i;

	i = 0;
	if (!startNode->haslogical)
	{
		is_last_command_btree(startNode->children[0]->left);
	}
	else
	{
		while (i < startNode->childcount)
		{
			is_last_command_btree(startNode->children[i]->left);
			if (i == 0 && startNode->children[i]->right)
			{
				is_last_command_btree(startNode->children[i]->right);
			}
			i++;
		}
	}
}

void	expenser(t_startnode *startNode)
{
	is_last_command(startNode);
}
