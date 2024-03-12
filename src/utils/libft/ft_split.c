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

static int	ft_allocate(char **tab, char const *s, char sep, size_t *h_error)
{
	char		**tab_p;
	char const	*tmp;

	tmp = s;
	tab_p = tab;
	while (*tmp)
	{
		while (*s == sep)
			s++;
		tmp = s;
		while (*tmp && *tmp != sep)
			tmp++;
		if (*tmp == sep || tmp > s)
		{
			*tab_p = ft_substr(s, 0, tmp - s);
			if (!*tab_p)
				return (0);
			s = tmp;
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

	word_count = 0;
	while (*s)
	{
		while (*s == sep)
			s++;
		if (*s)
			word_count++;
		while (*s && *s != sep)
			s++;
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

/*#include <stdio.h>
int	main(void)
{
	char **test = ft_split("salut tout le monde", ' ');
	for (int i = 0; i < 4; i++)
		printf("word %d : %s\n", i, test[i]);
	return (0);
}*/
