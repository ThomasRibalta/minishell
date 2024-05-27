#include "../../header/minishell.h"

char *get_cwd(int i)
{
    char *cwd;
    
    cwd = malloc(1024);
    if (!cwd) 
        return ("Erreur");
    if (!getcwd(cwd, 1024))
        return ("Erreur");
    if (i == 1 && ft_strlen(cwd) >= ft_strlen(getenv("HOME")))
    {
        cwd = ft_strjoin("~", cwd + ft_strlen(getenv("HOME")));
    }
    return (cwd);
}