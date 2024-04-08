#include "../../header/minishell.h"

void unset_var(char ***env, const char *name)
{
    char **current;
    char **next;
    
    current = *env;
    while (*current)
    {
        if (strncmp(*current, name, strlen(name)) == 0 && (*current)[strlen(name)] == '=')
        {
            free(*current);
            next = current + 1;
            while (*next)
            {
                *(next - 1) = *next;
                next++;
            }
            *(next - 1) = NULL;
            return;
        }
        current++;
    }
}