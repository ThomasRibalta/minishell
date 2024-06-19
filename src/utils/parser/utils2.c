/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:34:58 by toto              #+#    #+#             */
/*   Updated: 2024/06/11 21:21:05 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

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
	char	*tmp2;

	word = ft_substr(tmp + i + 1, 0, *j);
	tmp2 = ft_strjoin(ft_strdup(word), ft_strdup("="));
	while ((in_env(tmp2, env) != 1 && tmp[i + *j] && tmp[i + *j] != ' ' && tmp[i
				+ *j] != '$'))
	{
		free(word);
		free(tmp2);
		word = ft_substr(tmp + i + 1, 0, *j);
		(*j)++;
		tmp2 = ft_strjoin(ft_strdup(word), ft_strdup("="));
	}
	free(tmp2);
	return (word);
}
