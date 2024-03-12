/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 14:04:20 by jedurand          #+#    #+#             */
/*   Updated: 2023/10/03 14:06:49 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst -> next)
		lst = lst -> next;
	return (lst);
}

/*#include <stdio.h>
int	main(void)
{
	t_list *l = ft_lstnew("salut");
	t_list *l2 = ft_lstnew("bonjour");
	t_list *l3 = ft_lstnew("42");
	ft_lstadd_front(&l, l2);
	ft_lstadd_front(&l2, l3);
	t_list *result = ft_lstlast(l3);
	printf("%s", (char *)result->content);
	return (0);
}*/
