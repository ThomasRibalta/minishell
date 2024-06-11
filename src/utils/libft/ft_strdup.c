/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:23:20 by jedurand          #+#    #+#             */
/*   Updated: 2024/06/11 21:22:56 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <string.h>

char	*ft_strdup(const char *str)
{
	char	*new;
	size_t	len;

	len = ft_strlen(str) + 1;
	new = (char *)malloc(sizeof(*new) * len);
	if (!new)
		return (NULL);
	return ((char *)ft_memcpy(new, str, len));
}

char	*ft_strndup(const char *s1, size_t n)
{
	char	*copy;
	size_t	len;

	len = ft_strlen(s1);
	if (n < len)
		len = n;
	copy = (char *)malloc(len + 1);
	if (copy == NULL)
		return (NULL);
	ft_strncpy(copy, s1, len);
	copy[len] = '\0';
	return (copy);
}
