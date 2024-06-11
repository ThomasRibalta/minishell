/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:58:47 by toto              #+#    #+#             */
/*   Updated: 2024/06/11 18:24:03 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

int	add_new_entry(char ***env, char *name, char *value)
{
	char	**current;
	int		num_entries;

	current = *env;
	num_entries = 0;
	while (*current)
	{
		num_entries++;
		current++;
	}
	init_new_env(env, num_entries, name, value);
	return (0);
}

int	export_var2(char ***env, char *name, char *value)
{
	char	**current;

	current = *env;
	if (error_name(name))
		return (1);
	while (*current)
	{
		if (update_existing_entry(current, name, value) == 0)
			return (0);
		current++;
	}
	return (add_new_entry(env, name, value));
}

int	get_egal(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (i);
		i++;
	}
	return (i);
}

int	export_var(char ***env, char ***export, char **tab)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (tab[i])
	{
		if (!ft_strchr(tab[i], '='))
			status = export_var2(export, ft_strdup(tab[i]), ft_strdup(""));
		else
		{
			status = export_var2(env, ft_strndup(tab[i], get_egal(tab[i])),
					ft_strdup(tab[i] + (get_egal(tab[i]) + 1)));
			status = export_var2(export, ft_strndup(tab[i], get_egal(tab[i])),
					ft_strdup(tab[i] + (get_egal(tab[i]) + 1)));
		}
		i++;
	}
	return (status);
}
