/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:23:36 by jedurand          #+#    #+#             */
/*   Updated: 2023/10/04 11:19:03 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *str, int c, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		*((char *)(str + i)) = c;
		i++;
	}
	return (str);
}

/*#include <stdio.h>
int	main(void)
{
	char	test[6] = "salut";
	ft_memset(test, 'w', 4);
	printf("%s", test);
	return (0);
}*/
