#include "../../header/minishell.h"

char *get_cwd(int i)
{
    char *cwd;
    
    cwd = malloc(1024);
    if (!cwd) 
        return ("Erreur");
    if (!getcwd(cwd, 1024))
        return ("");
    if (i == 1 && ft_strlen(cwd) >= ft_strlen(getenv("HOME")))
    {
        cwd = ft_strjoin("~", cwd + ft_strlen(getenv("HOME")));
    }
    return (cwd);
}

char *pwd(int i)
{
    char *cwd = get_cwd(i);
    if (cwd[0] == '\0')
    {
        return("pwd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n");
    }else
        return (cwd);
}