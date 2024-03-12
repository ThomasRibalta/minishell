/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 14:07:06 by jedurand          #+#    #+#             */
/*   Updated: 2023/10/03 14:12:04 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*tmp;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp -> next)
		tmp = tmp -> next;
	tmp -> next = new;
}

/*#include <stdio.h>
int	main(void)
{
	t_list *l = ft_lstnew("salut");
	t_list *l2 = ft_lstnew("bonjour");
	t_list *l3 = ft_lstnew("42");
	t_list *end = ft_lstnew("the end");
	ft_lstadd_front(&l, l2);
	ft_lstadd_front(&l2, l3);
	ft_lstadd_back(&l3, end);
	t_list *result = ft_lstlast(l3);
	printf("%s", (char *)result->content);
	return (0);
}*/
