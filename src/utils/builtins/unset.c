/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:58:00 by toto              #+#    #+#             */
/*   Updated: 2024/06/11 17:30:43 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

int	unset_var2(char ***env, const char *name)
{
	char	**current;
	char	**next;

	current = *env;
	if (error_name(name) == 1)
		return (1);
	while (*current)
	{
		if (ft_strncmp(*current, name, ft_strlen(name)) == 0
			&& (*current)[ft_strlen(name)] == '=')
		{
			free(*current);
			next = current + 1;
			while (*next)
			{
				*(next - 1) = *next;
				next++;
			}
			*(next - 1) = NULL;
			return (1);
		}
		current++;
	}
	return (0);
}

int	unset_var(char ***env, char ***export, char **tab)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (tab[i])
	{
		unset_var2(env, tab[i]);
		unset_var2(export, tab[i]);
		i++;
	}
	return (status);
}
