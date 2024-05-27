#include "../../header/minishell.h"

void print_env(char **env)
{
    int i;

    i = 0;
    while (env[i] != NULL)
    {
        printf("%s\n", env[i]);
        i++;
    }
}