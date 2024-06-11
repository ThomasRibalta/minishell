/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:57:37 by toto              #+#    #+#             */
/*   Updated: 2024/06/11 12:48:35 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

int	error_name(const char *name)
{
	int	i;

	i = 0;
	if (!ft_isalpha(name[0]) && name[0] != '_')
	{
		printf("export: `%s` not a valid identifier\n", name);
		return (1);
	}
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
		{
			printf("export: `%s` not a valid identifier\n", name);
			return (1);
		}
		i++;
	}
	return (0);
}

int	update_existing_entry(char **current, char *name, char *value)
{
	if (ft_strncmp(*current, name, ft_strlen(name)) == 0)
	{
		free(*current);
		*current = ft_strjoin(name, ft_strdup("="));
		*current = ft_strjoin(*current, value);
		return (0);
	}
	return (-1);
}

char	**duplicate_env(char **env)
{
	char	**new_env;
	int		num_entries;
	int		i;

	new_env = NULL;
	num_entries = 0;
	i = -1;
	while (env[num_entries])
		num_entries++;
	new_env = malloc((num_entries + 1) * sizeof(char *));
	if (!new_env)
		return (NULL);
	while (++i < num_entries)
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
		{
			while (--i >= 0)
				free(new_env[i]);
			free(new_env);
			return (NULL);
		}
	}
	new_env[num_entries] = NULL;
	return (new_env);
}

void	init_new_env(char ***env, int num_entries, char *name, char *value)
{
	char	**new_env;
	int		i;

	i = 0;
	new_env = malloc((num_entries + 2) * sizeof(char *));
	if (!new_env)
		return ;
	while (i < num_entries)
	{
		new_env[i] = ft_strdup((*env)[i]);
		i++;
	}
	new_env[i] = ft_strjoin(name, ft_strdup("="));
	new_env[i] = ft_strjoin(new_env[i], value);
	new_env[i + 1] = NULL;
	free_tab(*env);
	*env = new_env;
	if (!new_env)
		return ;
	if (!new_env[num_entries])
	{
		free(new_env);
		return ;
	}
}
