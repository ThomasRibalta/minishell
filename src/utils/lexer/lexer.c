/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 14:18:49 by toto              #+#    #+#             */
/*   Updated: 2024/06/11 21:19:29 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

char	**split_with_symbols(const char *input)
{
	char		**result;
	const char	*start;
	int			i;
	int			count;
	int			symbol_length;

	start = input;
	result = (char **)malloc(sizeof(char *) * (count_tokens(input) + 1));
	i = 0;
	count = 0;
	while (input[i])
	{
		if (is_symbol(input + i, &symbol_length)
			&& !in_special_zone((char *)input, i))
		{
			handle_symbol(input + i, &start, result, &count);
			i += symbol_length;
		}
		else
			i++;
	}
	if (input + i > start)
		result[count++] = create_token(start, input + i);
	result[count] = NULL;
	return (result);
}

void	chained_split_prompt(t_token **list, char **tab)
{
	int	j;

	j = 0;
	while (tab[j])
	{
		if (tab[j][0] == '(')
			appendtoken(list, TOKEN_PAREN, tab[j]);
		else if (tab[j][0] == '|' && tab[j][1] == '\0')
			appendtoken(list, TOKEN_PIPE, tab[j]);
		else if (tab[j][0] == '<' && tab[j][1] == '\0')
			appendtoken(list, TOKEN_IN, tab[++j]);
		else if (tab[j][0] == '>' && tab[j][1] == '\0')
			appendtoken(list, TOKEN_OUT, tab[++j]);
		else if (tab[j][0] == '>' && tab[j][1] == '>')
			appendtoken(list, TOKEN_APPEND, tab[++j]);
		else if (tab[j][0] == '<' && tab[j][1] == '<')
			appendtoken(list, TOKEN_HEREDOC, here_doc(tab[++j]));
		else if (tab[j][0] == '&' && tab[j][1] == '&')
			appendtoken(list, TOKEN_LOGICAL_AND, tab[j]);
		else if (tab[j][0] == '|' && tab[j][1] == '|')
			appendtoken(list, TOKEN_LOGICAL_OR, tab[j]);
		else
			appendtoken(list, TOKEN_COMMAND, tab[j]);
		j++;
	}
}

int	condition(char **tab, int i)
{
	if (((tab[i][0] == '<' && (tab[i][1] == '\0'
			|| tab[i][1] == '<')) || (tab[i][0] == '>'
			&& (tab[i][1] == '\0' || tab[i][1] == '>'))) && tab[i + 1])
		return (1);
	return (0);
}

char	**re_build_with_redir(char **tab)
{
	char	**split_input;
	char	**tmptab;
	int		i;

	i = -1;
	while (tab[++i])
	{
		if (condition(tab, i))
		{
			split_input = ft_split(tab[i + 1], ' ');
			if (len_tab(split_input) > 1)
			{
				tmptab = malloc(sizeof(char *) * 3);
				tmptab[0] = ft_strdup(tab[i]);
				tmptab[1] = ft_strdup(split_input[0]);
				tmptab[2] = NULL;
				free(tab[i + 1]);
				tab[i + 1] = fuse_split_result(split_input, 1);
				tab = fusionner_tableaux(tab, tmptab, i);
			}
			free_tab(split_input);
		}
	}
	return (tab);
}

void	lexer(char *input, t_mainstruct mainstruct)
{
	char	**tab_input;
	char	**symbols;
	t_token	*list;

	list = NULL;
	if (!input || !check_input_error(input))
		return ;
	symbols = init_symbols();
	tab_input = split_with_symbols(input);
	tab_input = re_build_with_redir(tab_input);
	tab_input = tab_clean(tab_input);
	if (contains_invalid_sequences(tab_input, len_tab(tab_input), symbols))
	{
		*mainstruct.exit_status = 2;
		free_tab(tab_input);
		return ;
	}
	chained_split_prompt(&list, tab_input);
	free_tab(tab_input);
	free(symbols);
	parser(list, mainstruct);
	freetokens(list);
}
