/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gestion_erreur.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thomas.rba <thomas.rba@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 14:44:59 by toto              #+#    #+#             */
/*   Updated: 2024/06/30 19:59:37 by thomas.rba       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

char	**tab_clean(char **tab)
{
	char	**new_tab;
	int		j;
	int		i;

	j = 0;
	i = 0;
	new_tab = malloc(sizeof(char *) * (len_tab(tab) + 1));
	while (tab[i])
	{
		if (is_only_spaces_and_tabs(tab[i]))
		{
			free(tab[i]);
			i++;
		}
		new_tab[j] = clean_white_space(tab[i]);
		j++;
		i++;
	}
	free(tab);
	new_tab[j] = 0;
	return (new_tab);
}

int	check_input_error(char *input)
{
	int	i;

	i = 0;
	if ((input[0] == ':' || input[0] == '#') && input[1] == '\0')
		return (0);
	else if (input[0] == '!' && input[1] == '\0')
		return (0);
	else if (input[0] == '\0')
		return (0);
	while (input[i] == ' ' || input[i] == '\t')
		i++;
	if (input[i] == '\n')
		return (0);
	return (1);
}

bool	check_sequences(int i, int size, char **tab, char **symbols)
{
	int	k;

	if (i == size - 1)
	{
		ft_putstr_fd("syntax error near unexpected token `", 2);
		ft_putstr_fd(tab[i], 2);
		ft_putstr_fd("'\n", 2);
		return (true);
	}
	k = 0;
	while (k < 7)
	{
		if (ft_strcmp(tab[i + 1], symbols[k]) == 0)
		{
			ft_putstr_fd("syntax error near unexpected token `", 2);
			ft_putstr_fd(tab[i + 1], 2);
			ft_putstr_fd("'\n", 2);
			return (true);
		}
		k++;
	}
	return (false);
}

bool	check_special(char **tab, int i)
{
	if (tab[i + 1] && (ft_strcmp(tab[i + 1], ">") == 0
			|| ft_strcmp(tab[i + 1], ">>") == 0))
	{
		return (true);
	}
	return (false);
}

bool	contains_invalid_sequences(char **tab, int size, char **symbols)
{
	int	i;
	int	j;

	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < 7)
		{
			if (ft_strcmp(tab[i], symbols[j]) == 0)
			{
				if (ft_strcmp(tab[i], "|") == 0 && check_special(tab, i))
					break ;
				else if (check_sequences(i, size, tab, symbols))
					return (true);
			}
			j++;
		}
		i++;
	}
	return (false);
}
