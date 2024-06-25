/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:58:18 by toto              #+#    #+#             */
/*   Updated: 2024/06/11 17:19:59 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

int	is_only_n(const char *str)
{
	if (str == NULL || str[0] == '\0')
		return (0);
	while (*str != '\0')
	{
		if (*str != 'n')
			return (0);
		str++;
	}
	return (1);
}

void	echo(char **tab)
{
	int	i;

	i = 1;
	while (tab[i] && tab[i][0] == '-' && is_only_n(tab[i] + 1) == 1)
		i++;
	while (tab[i] != NULL)
	{
		ft_putstr_fd(tab[i], 1);
		if (i + 1 < len_tab(tab))
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (tab[1] && is_only_n(tab[1] + 1) != 1)
		ft_putchar_fd('\n', 1);
}
