#include "../../header/minishell.h"

char *get_cwd()
{
    char *cwd;
    
    cwd = malloc(1024);
    if (!cwd) 
        return ("Erreur");
    if (!getcwd(cwd, 1024))
        return ("Erreur");
    if (!ft_strncmp(cwd, getenv("HOME"), ft_strlen(cwd)))
    {
        free(cwd);
        cwd = malloc(2 * sizeof(char));
        if (!cwd)
            return ("Erreur");
        else
        {
            cwd[0] = '~';
            cwd[1] = '\0';
        }
    }
    return (cwd);
}