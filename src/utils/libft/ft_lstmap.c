/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 14:40:27 by jedurand          #+#    #+#             */
/*   Updated: 2023/10/03 16:13:34 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*head;
	t_list	*tmp;
	void	*new_content;

	head = NULL;
	while (lst)
	{
		new_content = (*f)(lst->content);
		tmp = ft_lstnew(new_content);
		if (!tmp)
		{
			del(new_content);
			ft_lstclear(&head, del);
			return (NULL);
		}
		ft_lstadd_back(&head, tmp);
		lst = lst -> next;
	}
	return (head);
}

/*static void	supp(void *content)
{
	free(content);
}

static void	*up(void *s)
{
	if (!s)
		return (NULL);
	char *str = ft_strdup(s);
	int i = -1;
	while (str[++i])
	{
		str[i] = ft_toupper(str[i]);
	}
	return (str);
}

static void	pstr(void *str)
{
	if (!str)
		return ;
	write(0, str, ft_strlen(str));
	write(0, "\n", 1);
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
	t_list *result = ft_lstmap(start, &up, &supp);
	ft_lstiter(result, &pstr);
	return (0);
}*/
