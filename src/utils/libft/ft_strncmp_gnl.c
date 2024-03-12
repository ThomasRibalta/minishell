/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp_gnl.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <jedurand@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 01:39:10 by jedurand          #+#    #+#             */
/*   Updated: 2024/02/21 01:39:56 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Compares two strings up to 'n' characters, specifically designed for strings
 *  returned by get_next_line.
 * It compares 's1' and 's2' character by character. If 's1' ends with a newline
 *  character followed by a null character,
 * this newline is effectively ignored in the comparison. This function is
 *  useful for comparing a get_next_line
 * output with a delimiter or another string, handling the newline
 *  character correctly.
 *
 * @param s1 The first string to be compared, typically from get_next_line.
 * @param s2 The second string to be compared, usually a
 *  delimiter or target string.
 * @param n The maximum number of characters to compare.
 * @return 0 if the strings are equal up to 'n' characters (ignoring a trailing
 *  newline in 's1'),
 *         or 1 if unmatched.
 */

int	ft_strncmp_gnl(const char *s1, const char *s2, size_t n)
{
	size_t		i;

	i = 0;
	while (i < n && s1[i] && s2[i] && s1[i] == s2[i])
	{
		i++;
	}
	if (i == n && s1[i] == '\n' && s1[i + 1] == '\0')
		return (0);
	else
		return (1);
}
