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
			path = ft_strjoin(pwd, "/");
			path = ft_strjoin(path, tab[0]);
			free(pwd);
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

static int	handle_cd_errors()
{
	char	*pwd;
    
    pwd = get_cwd(0);
	if (pwd[0] == '\0')
	{
		ft_putstr_fd("cd: error retrieving current directory: ", 2);
		ft_putstr_fd("getcwd: cannot access parent directories: ", 2);
		ft_putstr_fd("No such file or directory\n", 2);
		free(pwd);
		return (1);
	}
	free(pwd);
	return (0);
}

int	cd(char **tab, char ***env)
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
	export_var2(env, "OLDPWD", get_cwd(0));
	export_var2(env, "PWD", path);
	free(path);
	return (0);
}
