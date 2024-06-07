#include "../../header/minishell.h"

char **init_symbols()
{
    char **symbols;
    
    symbols = (char **)malloc(sizeof(char *) * 7);
    symbols[0] = "||";
    symbols[1] = "&&";
    symbols[2] = ">>";
    symbols[3] = ">";
    symbols[4] = "<<";
    symbols[5] = "<";
    symbols[6] = "|";
    return (symbols);
}

int is_symbol(const char *str, int *symbol_length)
{
    int i;
    int len;
    char **symbols;

    i = 0;
    symbols = init_symbols();
    while (i < 7)
    {
        len = ft_strlen(symbols[i]);
        if (ft_strncmp(str, symbols[i], len) == 0)
        {
            *symbol_length = len;
            free(symbols);
            return (1);
        }
        i++;
    }
    free(symbols);
    return (0);
}

void handle_symbol(const char *current, const char **start, char **result, int *count)
{
    int symbol_length;

    symbol_length = 0;
    is_symbol(current, &symbol_length);
    if (current > *start)
    {
        result[*count] = create_token(*start, current);
        (*count)++;
    }
    result[*count] = create_token(current, current + symbol_length);
    (*count)++;
    *start = current + symbol_length;
}
