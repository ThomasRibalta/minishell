/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 14:19:01 by jedurand          #+#    #+#             */
/*   Updated: 2023/10/03 15:01:57 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	(*del)(lst -> content);
	free(lst);
}

/*void	supp(void *content)
{
	free(content);
}

#include <stdio.h>
int	main(void)
{
	char *cont = ft_strdup("salut");
	t_list *l = ft_lstnew(cont);
	printf("%s", (char *)l->content);
	ft_lstdelone(l, &supp);
	printf("%s", (char *)l->content);
	return (0);
}*/
