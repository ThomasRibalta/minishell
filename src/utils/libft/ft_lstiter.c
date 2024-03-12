/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 14:36:14 by jedurand          #+#    #+#             */
/*   Updated: 2023/10/03 14:40:15 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	while (lst)
	{
		(*f)(lst -> content);
		lst = lst -> next;
	}
}
/*static void	pstr(void *str)
{
	if (!str)
		return ;
	write(0, str, ft_strlen(str));
}

#include <stdio.h>
int	main(void)
{
	t_list *l = ft_lstnew(ft_strdup("salut"));
	t_list *l2 = ft_lstnew(ft_strdup("bonjour"));
	t_list *l3 = ft_lstnew(ft_strdup("42"));
	t_list *end = ft_lstnew(ft_strdup("the end"));
	ft_lstadd_front(&l, l2);
	ft_lstadd_front(&l2, l3);
	ft_lstadd_back(&l3, end);
	t_list *start = l3;
	ft_lstiter(start, &pstr);
	return (0);
}*/
