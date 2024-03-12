/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:24:46 by jedurand          #+#    #+#             */
/*   Updated: 2023/10/02 18:04:26 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nitems, size_t size)
{
	void	*ptr;

	ptr = malloc(nitems * size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, nitems * size);
	return (ptr);
}

/*#include <stdio.h>
int	main(void)
{
	int *test = ft_calloc(4, sizeof(int));
	for (int i = 0; i < 4; i++)
		printf("%d\n", test[i]);
	return (0);
}*/
