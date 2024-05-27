#include "../../header/minishell.h"

void exit_program(char *value)
{
    if (value == NULL)
        exit(0);
    exit(ft_atoi(value));
}