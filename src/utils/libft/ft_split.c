/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 17:01:46 by jedurand          #+#    #+#             */
/*   Updated: 2023/10/03 11:55:44 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

int in_special_zone(char *input, int i) 
{
    int j;
    int open_double_quote;
    int open_single_quote;
    int open_parenthesis;

    j = 0;
    open_double_quote = 0;
    open_single_quote = 0;
    open_parenthesis = 0;
    while (input[j] && j != i)
    {
        if (input[j] == '"') 
        {
            if (!open_single_quote && !open_parenthesis) 
            {
                if (!open_double_quote)
                    open_double_quote++;
                else
                    open_double_quote--;
            }
        } 
        else if (input[j] == '\'') 
        {
            if (!open_double_quote && !open_parenthesis) 
            {
                if (!open_single_quote)
                    open_single_quote++;
                else
                    open_single_quote--;
            }
        }
        else if (input[j] == '(') 
        {
            if (!open_double_quote && !open_single_quote) 
                    open_parenthesis++;
        }
        else if (input[j] == ')') 
        {
            if (!open_double_quote && !open_single_quote) 
            {
                if (open_parenthesis)
                    open_parenthesis--;
            }
        }
        j++;
    }
		if (open_double_quote)
			return (1);
		if (open_single_quote)
			return (2);
		if (open_parenthesis)
			return (3);
		return (0);
}

static int	ft_allocate(char **tab, char const *s, char sep, size_t *h_error)
{
	char		**tab_p;
	char const	*tmp;
	int i;
	int j;

	tmp = s;
	tab_p = tab;
	i = 0;
	while (tmp[i])
	{
		j = 0;
		while (s[j] == sep)
			j++;
		if (s[j] == '\0')
			break ;
		tmp = s + j;
		i = 0;
		while (tmp[i] && (tmp[i] != sep || in_special_zone((char *)tmp, i)))
			i++;
		if (tmp[i] == sep || i > j || tmp[i] == '\0')
		{
			*tab_p = ft_substr(s + j, 0, i);
			if (!*tab_p)
				return (0);
			s = tmp + i;
			tab_p++;
			(*h_error)++;
		}
	}
	*tab_p = NULL;
	return (1);
}

static int	ft_count_words(char const *s, char sep)
{
	int	word_count;
	int i;

	word_count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == sep)
			i++;
		if (s[i] && in_special_zone((char *)s, i) == 0)
			word_count++;
		while (s[i] && s[i] != sep)
			i++;
	}
	return (word_count);
}

char	**ft_split(char const *s, char c)
{
	char	**new;
	int		size;
	size_t	h_error;

	if (!s)
		return (NULL);
	h_error = 0;
	size = ft_count_words(s, c);
	new = (char **)malloc(sizeof(char *) * (size + 1));
	if (!new)
		return (NULL);
	new[size] = 0;
	if (!ft_allocate(new, s, c, &h_error))
	{
		while (h_error > 0)
		{
			free(new[h_error - 1]);
			h_error--;
		}
		free(new);
		return (NULL);
	}
	return (new);
}

// #include <stdio.h>
// int	main(void)
// {
// 	char **test = ft_split("salut ", ' ');
// 	for (int i = 0; i < 4; i++)
// 		printf("word %d : %s\n", i, test[i]);
// 	return (0);
// }
