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

void	update_quote_states(char c, int *open_double_quote, 
	int *open_single_quote, int *open_parenthesis)
{
	if (c == '"')
	{
		if (!*open_single_quote && !*open_parenthesis)
		{
			if (!*open_double_quote)
				*open_double_quote = 1;
			else
				*open_double_quote = 0;
		}
	}
	else if (c == '\'')
	{
		if (!*open_double_quote && !*open_parenthesis)
		{
			if (!*open_single_quote)
				*open_single_quote = 1;
			else
				*open_single_quote = 0;
		}
	}
}

void	update_parenthesis_state(char c, int *open_double_quote, 
	int *open_single_quote, int *open_parenthesis)
{
	if (c == '(')
	{
		if (!*open_double_quote && !*open_single_quote)
			(*open_parenthesis)++;
	}
	else if (c == ')')
	{
		if (!*open_double_quote && !*open_single_quote && *open_parenthesis)
			(*open_parenthesis)--;
	}
}

int	check_special_zone_states(int open_double_quote, 
	int open_single_quote, int open_parenthesis)
{
	if (open_double_quote)
		return (1);
	if (open_single_quote)
		return (2);
	if (open_parenthesis)
		return (3);
	return (0);
}

int	in_special_zone(char *input, int i)
{
	int	j;
	int	open_double_quote;
	int	open_single_quote;
	int	open_parenthesis;

	j = 0;
	open_double_quote = 0;
	open_single_quote = 0;
	open_parenthesis = 0;
	while (input[j] && j != i)
	{
		update_quote_states(input[j], &open_double_quote, 
			&open_single_quote, &open_parenthesis);
		update_parenthesis_state(input[j], &open_double_quote, 
			&open_single_quote, &open_parenthesis);
		j++;
	}
	return (check_special_zone_states(open_double_quote, 
		open_single_quote, open_parenthesis));
}

static int	skip_separators(const char **s, char sep)
{
	int	j;

	j = 0;
	while ((*s)[j] == sep)
		j++;
	return (j);
}

static int	find_substr_length(const char *s, char sep)
{
	int	i;

	i = 0;
	while (s[i] && (s[i] != sep || in_special_zone((char *)s, i)))
		i++;
	return (i);
}

static int	allocate_and_assign(char **tab_p, const char *s, int len, size_t *h_error)
{
	*tab_p = ft_substr(s, 0, len);
	if (!*tab_p)
		return (0);
	(*h_error)++;
	return (1);
}

static int	ft_allocate(char **tab, char const *s, char sep, size_t *h_error)
{
	char		**tab_p;
	char const	*tmp;
	int			i;
	int			j;

	tab_p = tab;
	while (*s)
	{
		j = skip_separators(&s, sep);
		if (s[j] == '\0')
			break ;
		tmp = s + j;
		i = find_substr_length(tmp, sep);
		if (tmp[i] == sep || i > j || tmp[i] == '\0')
		{
			if (!allocate_and_assign(tab_p, s + j, i, h_error))
				return (0);
			s = tmp + i;
			tab_p++;
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
