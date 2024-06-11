/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 21:23:44 by toto              #+#    #+#             */
/*   Updated: 2024/06/11 21:29:30 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	update_quote_states(char c, int *open_double_quote,
		int *open_single_quote, int *open_parenthesis)
{
	if (c == '"')
	{
		if (!*open_single_quote && !*open_parenthesis)
		{
			if (!*open_double_quote)
				*open_double_quote = 1;
			else
				*open_double_quote = 0;
		}
	}
	else if (c == '\'')
	{
		if (!*open_double_quote && !*open_parenthesis)
		{
			if (!*open_single_quote)
				*open_single_quote = 1;
			else
				*open_single_quote = 0;
		}
	}
}

void	update_parenthesis_state(char c, int *open_double_quote,
		int *open_single_quote, int *open_parenthesis)
{
	if (c == '(')
	{
		if (!*open_double_quote && !*open_single_quote)
			(*open_parenthesis)++;
	}
	else if (c == ')')
	{
		if (!*open_double_quote && !*open_single_quote && *open_parenthesis)
			(*open_parenthesis)--;
	}
}

int	check_special_zone_states(int open_double_quote, int open_single_quote,
		int open_parenthesis)
{
	if (open_double_quote)
		return (1);
	if (open_single_quote)
		return (2);
	if (open_parenthesis)
		return (3);
	return (0);
}

int	in_special_zone(char *input, int i)
{
	int	j;
	int	open_double_quote;
	int	open_single_quote;
	int	open_parenthesis;

	j = 0;
	open_double_quote = 0;
	open_single_quote = 0;
	open_parenthesis = 0;
	while (input[j] && j != i)
	{
		update_quote_states(input[j], &open_double_quote, &open_single_quote,
			&open_parenthesis);
		update_parenthesis_state(input[j], &open_double_quote,
			&open_single_quote, &open_parenthesis);
		j++;
	}
	return (check_special_zone_states(open_double_quote, open_single_quote,
			open_parenthesis));
}
