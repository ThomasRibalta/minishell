#include "../../header/minishell.h"

void print_env(char **env, int nb)
{
    int i;

    i = 0;
    while (env[i] != NULL)
    {
        if (nb == 1)
            printf("declare -x ");
        printf("%s\n", env[i]);
        i++;
    }
}