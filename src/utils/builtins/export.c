/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:58:47 by toto              #+#    #+#             */
/*   Updated: 2024/06/08 15:58:48 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

int	add_new_entry(char ***env, const char *name, const char *value)
{
	char	**current;
	int		num_entries;
	char	**new_env;

	current = *env;
	num_entries = 0;
	while (*current)
	{
		num_entries++;
		current++;
	}
	new_env = init_new_env(env, num_entries, name, value);
	if (!new_env)
		return (1);
	ft_strcpy(new_env[num_entries], name);
	ft_strcpy(new_env[num_entries] + ft_strlen(name), "=");
	ft_strcpy(new_env[num_entries] + ft_strlen(name) + 1, value);
	new_env[num_entries + 1] = NULL;
	free(*env);
	*env = new_env;
	return (0);
}

int	export_var2(char ***env, const char *name, const char *value)
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

int	export_var(char ***env, char ***export, char **tab)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (tab[i])
	{
		if (!ft_strchr(tab[i], '='))
			status = export_var2(export, tab[i], "");
		else
		{
			status = export_var2(env, ft_strndup(tab[i], ft_strchr(tab[i], '=')
						- tab[i]), ft_strchr(tab[i], '=') + 1);
			status = export_var2(export, ft_strndup(tab[i], ft_strchr(tab[i],
							'=') - tab[i]), ft_strchr(tab[i], '=') + 1);
		}
		i++;
	}
	return (status);
}
