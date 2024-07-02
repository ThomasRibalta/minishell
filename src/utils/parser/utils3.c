/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thomas.rba <thomas.rba@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 14:54:36 by toto              #+#    #+#             */
/*   Updated: 2024/07/02 16:51:01 by thomas.rba       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

char	*replacesubstring(char *str, int start, int end, char *replacement)
{
	char	*new_str;
	char	*tmp;

	new_str = ft_strjoin(ft_substr(str, 0, start), replacement);
	tmp = ft_strjoin(new_str, ft_strdup(str + end));
	return (tmp);
}

void	replace_variable(char **tmp, int *i, char **env, int *exit_status)
{
	char	*word;
	char	*value;
	char	*new;
	int		j;

	j = 1;
	word = get_word_env(*tmp, *i, &j, env);
	if (in_env(word, env) || word[0] == '?')
	{
		if (word[0] == '?')
			value = ft_itoa(*exit_status);
		else
			value = get_env_value(word, env);
		free(word);
		if (value)
			*tmp = replacesubstring(*tmp, *i, *i + j, value);
	}
	else
	{
		new = ft_strjoin(ft_substr(*tmp, 0, *i), ft_strdup((*tmp) + *i + j));
		free(word);
		*tmp = new;
		(*i)--;
	}
}

void	replaceenvvars(char **str, char **env, int *exit_status)
{
	int		i;
	char	*tmp;
	char	*test;

	i = -1;
	tmp = *str;
	test = NULL;
	while (tmp[++i])
	{
		if (tmp[i] == '$' && in_special_zone(tmp, i) != 2 && tmp[i + 1]
			&& tmp[i + 1] != '$')
		{
			if (ft_strcmp(tmp, *str))
				test = tmp;
			replace_variable(&tmp, &i, env, exit_status);
			if (test)
				free(test);
		}
	}
	*str = tmp;
}

void	edit_shlvl(char **env)
{
	char	*tmp;

	while (*env)
	{
		if (ft_strncmp(*env, "SHLVL=", 6) == 0)
		{
			tmp = ft_strjoin(ft_strdup("SHLVL="), ft_itoa(ft_atoi(*env + 6)
						+ 1));
			free(*env);
			*env = tmp;
			break ;
		}
		env++;
	}
}
