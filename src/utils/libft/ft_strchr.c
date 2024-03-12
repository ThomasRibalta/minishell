/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:23:26 by jedurand          #+#    #+#             */
/*   Updated: 2023/10/03 11:03:04 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

int	ft_strnchr(const char *str, int c)
{
	char	*ptr;
	int	i;

	ptr = (char *)str;
	i = 0;
	while (ptr[i])
	{
		if (ptr[i] == (char)c)
			return (i);
		i++;
	}
	return (0);
}

char	*ft_strchr(const char *str, int c)
{
	char	*ptr;

	ptr = (char *)str;
	while (*ptr)
	{
		if (*ptr == (char)c)
			return (ptr);
		ptr++;
	}
	if ((char)c == 0)
		return (ptr);
	return (NULL);
}

/*#include <stdio.h>
int	main(void)
{
	char str[6] = "salut";
	printf("%p\n", str);
	printf("%p", ft_strchr(str, 1024));
	return (0);
}*/
