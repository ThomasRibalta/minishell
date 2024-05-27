#include "../../header/minishell.h"

void exit_program(char *value)
{
    int i;
    if (value == NULL)
        exit(0);
    i = ft_atoi(value);
    if (i >= 0 && i <= 255)
        exit(i);
    else
        exit(254);
}