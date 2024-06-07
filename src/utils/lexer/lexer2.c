#include "../../header/minishell.h"

char *fuse_split_result(char **split_input, int start)
{
    char *result;
    int i;

    i = start;
    result = ft_strdup(split_input[start]);
    while (split_input[i])
    {
        result = ft_strjoin(result, ft_strdup(" "));
        if (split_input[i + 1])
            result = ft_strjoin(result, ft_strdup(split_input[i + 1]));
        i++;
    }
    return (result);
}

int count_tokens(const char *input)
{
    int count = 0;
    const char *start = input;
    int i;
    int symbol_length;

    i = 0;
    while (input[i])
    {
        symbol_length = 0;
        if (is_symbol(input + i, &symbol_length) && !in_special_zone((char *)input, i))
        {
            if (input + i > start)
                count++;
            count++;
            i += symbol_length;
            start = input + i;
        }
        else
            i++;
    }
    if (input + i > start)
        count++;
    return count;
}

char *create_token(const char *start, const char *current)
{
    int len;
    len = current - start;
    char *token;
    token = (char *)malloc(len + 1);
    ft_strncpy(token, start, len);
    token[len] = '\0';
    return (token);
}

char *here_doc(char *end)
{
    char *value;
    char	*tmp;
	int		i;
    char *tmp2;

	i = 0;
    value = malloc(sizeof(char *));
    value[0] = '\0';
    tmp2 = clean_quote(ft_strdup(end));
	while (i != 1)
	{
		write(1, "Here_doc >> ", 13);
		tmp = get_next_line(0);
		if (ft_strncmp(tmp, tmp2, ft_strlen(end)) == 0)
			break ;
        value = ft_strjoin(value, tmp);
        free(tmp);
	}
    free(tmp2);
    return (value);
}
