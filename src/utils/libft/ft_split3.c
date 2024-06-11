/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 21:26:15 by toto              #+#    #+#             */
/*   Updated: 2024/06/11 21:27:39 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	skip_separators(const char **s, char sep)
{
	int	j;

	j = 0;
	while ((*s)[j] == sep)
		j++;
	return (j);
}

int	find_substr_length(const char *s, char sep)
{
	int	i;

	i = 0;
	while (s[i] && (s[i] != sep || in_special_zone((char *)s, i)))
		i++;
	return (i);
}
