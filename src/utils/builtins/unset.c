#include "../../header/minishell.h"

int unset_var2(char ***env, const char *name)
{
    char **current;
    char **next;
    
    current = *env;
    if (error_name(name) == 1)
        return (1);
    while (*current)
    {
        if (ft_strncmp(*current, name, ft_strlen(name)) == 0 && (*current)[ft_strlen(name)] == '=')
        {
            free(*current);
            next = current + 1;
            while (*next)
            {
                *(next - 1) = *next;
                next++;
            }
            *(next - 1) = NULL;
            return (1);
        }
        current++;
    }
    return (0);
}

int unset_var(char ***env, char **tab) 
{
    int i;
    int status;

    i = 0;
    status = 0;
    while(tab[i])
    {
        unset_var2(env, tab[i]);
        i++;
    }
    return (status);
}