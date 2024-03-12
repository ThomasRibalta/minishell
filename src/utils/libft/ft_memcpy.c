/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:23:57 by jedurand          #+#    #+#             */
/*   Updated: 2023/10/03 11:33:53 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	if (!dest && !src)
		return (NULL);
	i = 0;
	while (i < n)
	{
		((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dest);
}

/*if (!dest || !src)
		return (NULL);
check why && and not ||*/

/*#include <stdio.h>
int main(void)
{
	char src[6] = "salut";
	char dest[100];
	ft_memcpy(dest, src, 4);
	printf("%s", dest);
	return (0);
}*/
