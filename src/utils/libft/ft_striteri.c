/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 18:00:01 by jedurand          #+#    #+#             */
/*   Updated: 2023/10/02 18:01:45 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	i = 0;
	if (!s)
		return ;
	while (s[i])
	{
		(*f)(i, (s + i));
		++i;
	}
}

/*static void	ft_upone(unsigned int index, char *c)
{
	*c += 1 + index - index;
}
#include <stdio.h>
int	main (void)
{
	char str[6] = "salut";
	ft_striteri(str, &ft_upone);
	printf("%s", str);
	return (0);
}*/
