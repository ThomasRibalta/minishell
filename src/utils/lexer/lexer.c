#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../header/minishell.h"

char **split_with_symbols(const char *input)
{
    char **result;
    const char *start;
    int i;
    int count;
    int symbol_length;

    start = input;
    result = (char **)malloc(sizeof(char *) * (count_tokens(input) + 1));
    i = 0;
    count = 0;
    while (input[i])
    {
        symbol_length = 0;
        if (is_symbol(input + i, &symbol_length) && !in_special_zone((char *)input, i))
        {
            handle_symbol(input + i, &start, result, &count);
            i+= symbol_length;
        }
        else
            i++;
    }
    if (input + i > start) 
        result[count++] = create_token(start, input + i);
    result[count] = NULL;
    return (result);
}

void chained_split_prompt(Token **list, char **tab)
{
    int j;

    j = 0;
    while (tab[j])
    {
        if (tab[j][0] == '(')
            appendToken(list, TOKEN_PAREN, tab[j]);
        else if (tab[j][0] == '|' && tab[j][1] == '\0')
            appendToken(list, TOKEN_PIPE, tab[j]);
        else if (tab[j][0] == '<' && tab[j][1] == '\0')
            appendToken(list, TOKEN_IN, tab[++j]);
        else if (tab[j][0] == '>' && tab[j][1] == '\0')
            appendToken(list, TOKEN_OUT, tab[++j]);
        else if (tab[j][0] == '>' && tab[j][1] == '>')
            appendToken(list, TOKEN_APPEND, tab[++j]);
        else if (tab[j][0] == '<' && tab[j][1] == '<')
            appendToken(list, TOKEN_HEREDOC, here_doc(tab[++j]));
        else if (tab[j][0] == '&' && tab[j][1] == '&')
            appendToken(list, TOKEN_LOGICAL_AND, tab[j]);
        else if (tab[j][0] == '|' && tab[j][1] == '|')
            appendToken(list, TOKEN_LOGICAL_OR, tab[j]);
        else
            appendToken(list, TOKEN_COMMAND, tab[j]);
        j++;
    }
}

char **re_build_with_redir(char **tab_input)
{
    char **split_input;
    char **tmptab;
    int i;

    i = 0;
    while (tab_input[i])
    {
        if (((tab_input[i][0] == '<' && (tab_input[i][1] == '\0' || tab_input[i][1] == '<')) 
        || (tab_input[i][0] == '>' && (tab_input[i][1] == '\0' || tab_input[i][1] == '>'))) && tab_input[i + 1])
        {
            split_input = ft_split(tab_input[i + 1], ' ');
            if (len_tab(split_input) > 1)
            {
                tmptab = malloc(sizeof(char *) * 3);
                tmptab[0] = ft_strdup(tab_input[i]);
                tmptab[1] = ft_strdup(split_input[0]);
                tmptab[2] = NULL;
                tab_input[i + 1] = fuse_split_result(split_input, 1);
                tab_input = fusionner_tableaux(tab_input, tmptab, i);
            }
            free_tab(split_input);
        }
        i++;
    }
    return (tab_input);
}

void lexer(char *input, char ***env, char ***export, int *exit_status)
{
    char **tab_input;
    char **symbols;
    Token *list = NULL;

    if (!input || !check_input_error(input))
        return ;
    symbols = init_symbols();
    tab_input = split_with_symbols(input);
    tab_input = re_build_with_redir(tab_input);
    tab_input = tab_clean(tab_input);
    if(contains_invalid_sequences(tab_input, len_tab(tab_input), symbols))
    {
        *exit_status = 2;
        free_tab(tab_input);
        return ;
    }
    chained_split_prompt(&list, tab_input);
    free_tab(tab_input);
    free(symbols);
    parser(list, env, export, exit_status);
    freeTokens(list);
}
