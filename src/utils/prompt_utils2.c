/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 14:16:37 by toto              #+#    #+#             */
/*   Updated: 2024/06/08 14:16:38 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

char	*clean_white_space(char *input)
{
	int		i;
	int		j;
	char	*clean_input;

	i = 0;
	j = ft_strlen(input);
	while (input[i] != '\0' && (!ft_isascii(input[i]) || input[i] == ' '))
		i++;
	while ((!ft_isascii(input[j - 1]) || input[j - 1] == ' ') && j != 0)
		j--;
	if (i >= j)
	{
		free(input);
		return (NULL);
	}
	clean_input = malloc((j - i + 1) * sizeof(char));
	if (!clean_input)
		return (NULL);
	clean_input[j - i] = '\0';
	j++;
	while (i != --j)
		clean_input[j - i - 1] = input[j - 1];
	free(input);
	return (clean_input);
}

char	*clean_quote(char *input)
{
	int	i;
	int	tmp;
	char *tmp_input;

	i = 0;
	tmp = 0;
	while (input[i])
	{
		if (input[i] == '"' && ft_strnchr(input + i + 1, '"') != -1)
		{
			tmp = i + 1 + ft_strnchr(input + i + 1, '"');
			tmp_input = ft_strjoin(ft_strjoin(ft_substr(input, 0, i),
						ft_substr(input, i + 1, ft_strnchr(input + i + 1,
								'"'))), ft_substr(input + 1, ft_strnchr(input
							+ i + 1, '"') + i + 1, ft_strlen(input)));
			free(input);
			input = tmp_input;
			i = tmp - 2;
		}
		else if (input[i] == 39 && ft_strnchr(input + i + 1, 39) != -1)
		{
			tmp = i + 1 + ft_strnchr(input + i + 1, 39);
			tmp_input = ft_strjoin(ft_strjoin(ft_substr(input, 0, i),
						ft_substr(input, i + 1, ft_strnchr(input + i + 1, 39))),
					ft_substr(input + 1, ft_strnchr(input + i + 1, 39) + i + 1,
						ft_strlen(input)));
			free(input);
			input = tmp_input;
			i = tmp - 2;
		}
		i++;
	}
	return (input);
}

char	*clean_prompt(char *input, t_mainstruct mainstruct)
{
	char	*prompt;

	prompt = clean_white_space(input);
	lexer(prompt, mainstruct);
	return (prompt);
}

void	check_prompt(char *input, t_mainstruct mainstruct)
{
	char	*prompt;

	prompt = clean_prompt(input, mainstruct);
	if (prompt)
		free(prompt);
}
