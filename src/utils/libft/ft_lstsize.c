/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 13:59:53 by jedurand          #+#    #+#             */
/*   Updated: 2023/10/04 10:39:04 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst-> next;
		i++;
	}
	return (i);
}

/*#include <stdio.h>
int	main(void)
{
	t_list *l = ft_lstnew("salut");
	t_list *l2 = ft_lstnew("bonjour");
	t_list *l3 = ft_lstnew("42");
	ft_lstadd_front(&l, l2);
	ft_lstadd_front(&l2, l3);
	printf("%d", ft_lstsize(l3));
	return (0);
}*/
