/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:58:55 by toto              #+#    #+#             */
/*   Updated: 2024/06/11 13:08:29 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static char	*get_target_path(char **tab, char **env)
{
	char	*path;
	char	*pwd;

	if (len_tab(tab) > 1)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (NULL);
	}
	else if (len_tab(tab) == 1)
	{
		if (tab[0][0] != '/')
		{
			pwd = get_cwd(0);
			path = ft_strjoin(pwd, ft_strdup("/"));
			path = ft_strjoin(path, ft_strdup(tab[0]));
		}
		else
			path = ft_strdup(tab[0]);
	}
	else
		path = get_env_value("HOME", env);
	return (path);
}

static int	change_directory(char *path)
{
	if (chdir(path) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
	return (0);
}

static int	handle_cd_errors(void)
{
	char	*pwd;

	pwd = get_cwd(0);
	if (pwd[0] == '\0')
	{
		if (pwd)
			free(pwd);
		ft_putstr_fd("cd: error retrieving current directory: ", 2);
		ft_putstr_fd("getcwd: cannot access parent directories: ", 2);
		ft_putstr_fd("No such file or directory\n", 2);
		return (1);
	}
	free(pwd);
	return (0);
}

int	cd(char **tab, char ***env, char ***export)
{
	char	*path;

	path = get_target_path(tab, *env);
	if (!path)
		return (1);
	if (handle_cd_errors())
	{
		free(path);
		return (1);
	}
	if (change_directory(path))
	{
		free(path);
		return (1);
	}
	export_var2(env, ft_strdup("OLDPWD"), get_cwd(0));
	export_var2(env, ft_strdup("PWD"), ft_strdup(path));
	export_var2(export, ft_strdup("OLDPWD"), get_cwd(0));
	export_var2(export, ft_strdup("PWD"), path);
	return (0);
}
