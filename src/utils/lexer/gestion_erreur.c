#include "../../header/minishell.h"

char **tab_clean(char **tab)
{
    char **new_tab;
    int     j;

    j = 0;
    new_tab = malloc(sizeof(char *) * (len_tab(tab) + 1));
    while (tab[j])
    {
        new_tab[j] = clean_white_space(tab[j]);
        j++;
    }
    free(tab);
    new_tab[j] = 0;
    return (new_tab);
}

int check_input_error(char *input)
{
    int i;

    i = 0;
    if ((input[0] == ':' || input[0] == '#') && input[1] == '\0')
        return (0);
    else if (input[0] == '!' && input[1] == '\0')
        return (0);
    else if (input[0] == '\0')
        return (0);
    while (input[i] == ' ' || input[i] == '\t')
            i++;
    if (input[i] == '\n')
        return (0);
    return (1);
}


bool check_sequences(int i, int size, char **tab, char **symbols)
{
    int k;
    if (i == size - 1)
    {
        ft_putstr_fd("syntax error near unexpected token `", 2);
        ft_putstr_fd(tab[i], 2);
        ft_putstr_fd("'\n", 2);
        return true;
        }
        k = 0;
        while (k < 7)
        {
            if (ft_strcmp(tab[i + 1], symbols[k]) == 0)
            {
            ft_putstr_fd("syntax error near unexpected token `", 2);
            ft_putstr_fd(tab[i + 1], 2);
            ft_putstr_fd("'\n", 2);
            return true;
        }
        k++;
    }
    return false;
}

bool contains_invalid_sequences(char **tab, int size, char **symbols)
{
    int i;
    int j;

    i = 0;
    while (i < size)
    {
        j = 0;
        while (j < 7)
        {
            if (ft_strcmp(tab[i], symbols[j]) == 0)
            {
                if (check_sequences(i, size, tab, symbols))
                    return true;
            }
            j++;
        }
        i++;
    }
    return false;
}