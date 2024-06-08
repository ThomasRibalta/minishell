/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:34:58 by toto              #+#    #+#             */
/*   Updated: 2024/06/08 15:35:00 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

void	addlogicalnodetostartnode(t_startnode *startnode, t_token *tokens)
{
	int			index;
	t_token		*ct;
	t_nodetype	type;

	if (startnode->haslogical)
	{
		index = 0;
		ct = tokens;
		while (ct != NULL && index < startnode->childcount)
		{
			if (ct->type == TOKEN_LOGICAL_AND || ct->type == TOKEN_LOGICAL_OR)
			{
				if (ct->type == TOKEN_LOGICAL_AND)
					type = NODE_LOGICAL_AND;
				else
					type = NODE_LOGICAL_OR;
				startnode->children[index] = createlogicalnode(type);
				index++;
			}
			ct = ct->next;
		}
	}
	else
		startnode->children[0] = createlogicalnode(NODE_LOGICAL_HOLDER);
}

int	in_env(char *word, char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(word, env[i], ft_strlen(word)) == 0)
			return (1);
		i++;
	}
	return (0);
}

char	*get_env_value(char *word, char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(word, env[i], ft_strlen(word)) == 0)
			return (ft_substr(env[i], ft_strlen(word) + 1, ft_strlen(env[i])
					- ft_strlen(word) - 1));
		i++;
	}
	return (NULL);
}

char	*get_word_env(char *tmp, int i, int *j, char **env)
{
	char	*word;

	word = ft_substr(tmp + i + 1, 0, *j);
	while ((in_env(ft_strjoin(word, ft_strdup("=")), env) != 1 && tmp[i + *j]
			&& tmp[i + *j] != ' ' && tmp[i + *j] != '$'))
	{
		free(word);
		word = ft_substr(tmp + i + 1, 0, *j);
		(*j)++;
	}
	return (word);
}
