/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execv.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:09:26 by toto              #+#    #+#             */
/*   Updated: 2024/06/08 16:09:28 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

char	*get_path(char **env)
{
	char	*path;

	path = get_env_value("PATH", env);
	return (path);
}

char	*check_path(char **paths, char *name)
{
	char	*join;
	char	*tmp;
	int		i;

	i = 0;
	if (name[0] == '.' && name[1] == '/')
		return (name);
	if (ft_strncmp(name, "/bin/", 5) == 0)
	{
		if (access(name, F_OK | X_OK) == 0)
			return (name);
	}
	if (!paths)
		return (NULL);
	while (paths[i])
	{
		tmp = ft_strjoin(ft_strdup(paths[i]), ft_strdup("/"));
		join = ft_strjoin(tmp, ft_strdup(name));
		if (access(join, F_OK | X_OK) == 0)
			return (join);
		free(join);
		i++;
	}
	return (NULL);
}

void	execute(char **param, char *path, char **env)
{
	char	**paths;
	char	*path1;

	if (param[0][0] == '/' && ft_strcmp(param[0], "/bin/") == 0)
	{
		ft_putendl_fd("minishell: is a directory", 2);
		exit(126);
	}
	paths = ft_split(path, ':');
	path1 = check_path(paths, clean_quote(param[0]));
	if (!path1)
	{
		ft_putendl_fd("minishell: command not found", 2);
		if (paths)
			free_tab(paths);
		exit(127);
	}
	if (execve(path1, param, env) == -1)
		free_tab(paths);
	free(path1);
	exit(EXIT_FAILURE);
}
