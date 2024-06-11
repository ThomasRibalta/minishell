/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:03:42 by jedurand          #+#    #+#             */
/*   Updated: 2024/06/11 18:08:44 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, int start, size_t len)
{
	char	*substr;
	size_t	size;

	if (!s)
		return (NULL);
	if ((size_t)start >= (size_t)ft_strlen(s))
		return (ft_strdup(""));
	size = ft_strlen(s + start);
	if (size < len)
		len = size;
	substr = (char *)malloc(sizeof(char) * (len + 1));
	if (!substr)
		return (NULL);
	ft_strlcpy(substr, s + start, len + 1);
	return (substr);
}

/*#include <stdio.h>
int	main(void)
{
	printf("%s", ft_substr("salut w test", 6, 4));
	return (0);
}*/
