/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 14:24:45 by jedurand          #+#    #+#             */
/*   Updated: 2023/10/03 14:35:24 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*head;
	t_list	*tmp;

	head = *lst;
	while (head)
	{
		tmp = head -> next;
		(*del)(head -> content);
		free(head);
		head = tmp;
	}
	*lst = NULL;
}

/*void	supp(void *content)
{
	free(content);
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
	ft_lstclear(&start, &supp);
	printf("%p", start);
	return (0);
}*/
