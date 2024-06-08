/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:23:20 by jedurand          #+#    #+#             */
/*   Updated: 2024/06/08 15:50:52 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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

char	*ft_strndup(const char *str, size_t n)
{
	char	*new;
	size_t	len;

	len = ft_strlen(str) + 1;
	if (n < len)
		len = n;
	new = (char *)malloc(sizeof(*new) * len);
	if (!new)
		return (NULL);
	return ((char *)ft_memcpy(new, str, len));
}

/*#include <stdio.h>
int	main(void)
{
	char *res = ft_strdup("salut");
	printf("%s", res);
	return (0);
}*/
