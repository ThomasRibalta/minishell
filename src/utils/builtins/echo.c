#include "../../header/minishell.h"

void echo(char **tab)
{
    int i = 0;
    if (ft_strncmp(tab[0], "-n", 2) == 0)
        i++;
    while (tab[i] != NULL)
    {
        printf("%s ", tab[i]);
        i++;
    }
    if (ft_strncmp(tab[0], "-n", 2) != 0)
        printf("\n");
}