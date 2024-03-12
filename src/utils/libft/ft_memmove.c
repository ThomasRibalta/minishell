/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:23:49 by jedurand          #+#    #+#             */
/*   Updated: 2023/10/02 12:23:52 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;
	size_t				i;

	d = (unsigned char *) dest;
	s = (const unsigned char *) src;
	i = -1;
	if (d == s)
		return (dest);
	if (dest < src)
	{
		while (++i < n)
			*(d++) = *(s++);
	}
	else
	{
		d += n - 1;
		s += n - 1;
		while (++i < n)
			*(d--) = *(s--);
	}
	return (dest);
}

/*#include <stdio.h>
int	main(void)
{
	char src[10] = "salut";
	ft_memmove(src + 2, src, 1);
	printf("%s", src);
	return (0);
}*/
