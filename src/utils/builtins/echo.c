#include "../../header/minishell.h"

void echo(char **tab)
{
    int i;
    
    i = 1;
    if (tab[1] == NULL || tab[1][0] == '\0')
        ft_putchar_fd('\n', 1);
    while (strcmp(tab[i], "-n") == 0)
        i++;
    while (tab[i] != NULL)
    {
        ft_putstr_fd(tab[i], 1);
        if (i + 1 < len_tab(tab))
            ft_putchar_fd(' ', 1);
        i++;
    }
    if (ft_strncmp(tab[1], "-n", 2) != 0)
        ft_putchar_fd('\n', 1);
}