#include "../../header/minishell.h"

int ft_strlen_tab(char **tab)
{
    int i;

    i = 0;
    while (tab[i])
        i++;
    return (i);
}

int cd(char **tab, char ***env)
{
    char *pwd = get_cwd(0);
    char *path;

    if (pwd[0] == '\0')
    {
        ft_putstr_fd("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n", 2);
        return (1);
    }
    if(ft_strlen_tab(tab) > 1){
        ft_putstr_fd("cd: too many arguments\n", 2);
        return (1);
    }
    if (ft_strlen_tab(tab) == 1)
    {
        if (tab[0][0] != '/')
        {
            path = ft_strjoin(pwd, "/");
            path = ft_strjoin(path, tab[0]);
        }
        else
            path = tab[0];
    }
    else
        path = get_env_value("HOME", *env, 0);
    if (chdir(path) == -1) {
        ft_putstr_fd("cd: ", 2);
        ft_putstr_fd(path, 2);
        ft_putstr_fd(": No such file or directory\n", 2);
        return (1);
    }
    export_var2(env, "OLDPWD", pwd);
    export_var2(env, "PWD", path);
    return (0);
}
