/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 17:01:46 by jedurand          #+#    #+#             */
/*   Updated: 2024/06/11 21:27:31 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static int	allocate_and_assign(char **tab_p, const char *s, int len,
		size_t *h_error)
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
	int	i;

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
