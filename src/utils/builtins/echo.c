#include "../../header/minishell.h"

void echo(char **tab)
{
    int i;
    
    i = 1;
    if (tab[1] == NULL || tab[1][0] == '\0')
        printf("\n");
    while (strcmp(tab[i], "-n") == 0)
        i++;
    while (tab[i] != NULL)
    {
        printf("%s", tab[i]);
        if (i + 1 < len_tab(tab))
            printf(" ");
        i++;
    }
    if (ft_strncmp(tab[1], "-n", 2) != 0)
        printf("\n");
}