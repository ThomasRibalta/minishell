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
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		join = ft_strjoin(tmp, name);
		free(tmp);
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
	if (!path)
	{
		ft_putendl_fd("minishell: No such file or directory", 2);
		exit(127);
	}
	paths = ft_split(path, ':');
	path1 = check_path(paths, clean_quote(param[0]));
	if (!path1 || !paths)
	{
		ft_putendl_fd("minishell: command not found", 2);
		free_tab(paths);
		exit(127);
	}
	if (execve(path1, param, env) == -1)
		free_tab(paths);
	free(path1);
	exit(EXIT_FAILURE);
}
