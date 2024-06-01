#include "../header/minishell.h"

char *clean_white_space(char *input)
{
    int i;
    int j;
    char *clean_input;

    i = 0;
    j = ft_strlen(input);
    while (input[i] != '\0' && (!ft_isascii(input[i]) || input[i] == ' '))
        i++;
    while ((!ft_isascii(input[j - 1]) || input[j - 1] == ' ') && j != 0)
        j--;
    if (i >= j)
    {
        free(input);
        return (NULL);
    }
    clean_input = malloc((j - i + 1) * sizeof(char));
    if (!clean_input)
        return (NULL);
    clean_input[j - i] = '\0';
    while (i != j)
    {
        clean_input[j - i - 1] = input[j - 1];
        j--;
    }
    free(input);
    return (clean_input);
}

char *clean_quote(char *input)
{
    int i;
    int tmp;

    i = 0;
    tmp = 0;
    while (input[i])
    {
        if (input[i] == '"' && ft_strnchr(input + i + 1, '"') != -1)
        {
            tmp = i + 1 + ft_strnchr(input + i + 1, '"');
            input = ft_strjoin(ft_strjoin(ft_substr(input, 0, i), ft_substr(input, i + 1, ft_strnchr(input + i + 1, '"'))),
            ft_substr(input + 1, ft_strnchr(input + i + 1, '"') + i + 1, ft_strlen(input)));
            i = tmp - 2;
        }
        else if (input[i] == 39 && ft_strnchr(input + i + 1, 39) != -1)
        {
            tmp = i + 1 + ft_strnchr(input + i + 1, 39);
            input = ft_strjoin(ft_strjoin(ft_substr(input, 0, i), ft_substr(input, i + 1, ft_strnchr(input + i + 1, 39))),
            ft_substr(input + 1, ft_strnchr(input + i + 1, 39) + i + 1, ft_strlen(input)));
            i = tmp - 2;
        }
        i++;
    }
    return (input);
}

char *clean_prompt(char *input, char ***env, char ***export, int *exit_status)
{
    char *prompt;

    prompt = clean_white_space(input);
    lexer(prompt, env, export, exit_status);
    return (prompt);
}

void check_prompt(char *input, char ***env, char ***export, int *exit_status)
{
    char *prompt;

    prompt = clean_prompt(input, env, export, exit_status);
    if (prompt)
        free(prompt);
}

char **clean_quote_all(char **split_nodeValue)
{
    int i;

    i = 0;
    while (split_nodeValue[i])
    {
        split_nodeValue[i] = clean_quote(split_nodeValue[i]);
        i++;
    }
    return split_nodeValue;
}