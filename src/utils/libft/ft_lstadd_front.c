/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 13:49:31 by jedurand          #+#    #+#             */
/*   Updated: 2023/10/03 13:57:20 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	t_list	*tmp;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	*lst = new;
	new -> next = tmp;
}

/*#include <stdio.h>
int	main(void)
{
	t_list *l = ft_lstnew("salut");
	t_list *l2 = ft_lstnew("bonjour");
	ft_lstadd_front(&l, l2);
	printf("%s", (char *)l2->next->content);
	return (0);
}*/
