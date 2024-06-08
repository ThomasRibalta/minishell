/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 14:13:40 by toto              #+#    #+#             */
/*   Updated: 2024/06/08 14:13:41 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

char	*remove_parenthese(char *str)
{
	char	*new_str;

	if (str[ft_strlen(str) - 1] == ')')
	{
		new_str = ft_substr(str, 1, ft_strlen(str) - 2);
	}
	else
	{
		new_str = ft_substr(str, 1, ft_strlen(str) - 1);
	}
	return (new_str);
}

char	**clean_quote_all(char **split_nodeValue)
{
	int	i;

	i = 0;
	while (split_nodeValue[i])
	{
		split_nodeValue[i] = clean_quote(split_nodeValue[i]);
		i++;
	}
	return (split_nodeValue);
}

int	len_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}
