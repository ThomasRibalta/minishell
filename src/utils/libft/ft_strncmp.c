/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:22:55 by jedurand          #+#    #+#             */
/*   Updated: 2024/06/08 15:52:52 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *str1, const char *str2, size_t n)
{
	unsigned char	*ptr1;
	unsigned char	*ptr2;

	ptr1 = (unsigned char *)str1;
	ptr2 = (unsigned char *)str2;
	while (n && *ptr1 && *ptr1 == *ptr2)
	{
		++ptr1;
		++ptr2;
		--n;
	}
	if (n)
		return (*ptr1 - *ptr2);
	else
		return (0);
}

int	ft_strcmp(char *value1, char *value2)
{
	while (*value1 && (*value1 == *value2))
	{
		value1++;
		value2++;
	}
	return (*(unsigned char *)value1 - *(unsigned char *)value2);
}
