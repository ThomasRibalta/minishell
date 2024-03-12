/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:24:52 by jedurand          #+#    #+#             */
/*   Updated: 2023/10/02 12:24:55 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *str, size_t n)
{
	ft_memset(str, 0, n);
}

/*#include <stdio.h>
int	main(void)
{
	char	test[6] = "salut";
	ft_bzero(test, 3);
	printf("%s", test);
	return (0);
}*/
