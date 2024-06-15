/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_terminal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 14:17:07 by toto              #+#    #+#             */
/*   Updated: 2024/06/11 21:19:02 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"
#include <readline/history.h>
#include <readline/readline.h>

void	write_terminal_title(void)
{
	printf("\033[2J");
	printf("\033[1;1H" VIOLET " __  __  _____  _   _  _____   " VIOLET_FONCE
		"_____  _    _  ______  _       _\n" RESET
		VIOLET "|  \\/||_   _|| \\ | ||_   _| "
		VIOLET_FONCE "/ ____|| | |||  ____|| |     | |\n"RESET
		VIOLET "| \\/ |  | |  |  \\| |  | |  " VIOLET_FONCE
		"| (___  | |__||| |__   | |     | |\n" RESET VIOLET
		"| |\\/| |  | |  | . ` |  | |  " VIOLET_FONCE
		" \\___ \\ |  __||  __|  | |     | |\n"
		RESET VIOLET "| |  | | _| |_ | |\\  | _| |_  " VIOLET_FONCE
		"____)|| | ||| |____ | |____ | |____\n" RESET VIOLET
		"|_|  |_||_____||_| \\_||_____|" VIOLET_FONCE
		"|_____/ |_|  |_||______||______||______|\n\n" RESET);
}

char	**clone_env(char **env)
{
	int		num_elements;
	char	**env_clone;
	int		i;

	num_elements = 0;
	i = 0;
	while (env[num_elements] != NULL)
	{
		num_elements++;
	}
	env_clone = malloc((num_elements + 1) * sizeof(char *));
	while (i < num_elements)
	{
		env_clone[i] = ft_strdup(env[i]);
		i++;
	}
	env_clone[num_elements] = NULL;
	return (env_clone);
}

void	init_terminal(char **env)
{
	edit_shlvl(env);
	write_terminal_title();
}

void	start_terminal(char **env, char **export, int exit_status)
{
	char			*input;
	char			*make_readline;
	t_mainstruct	mainstruct;
	int				exite;

	exite = -1;
	mainstruct = intialize_mainstruct(&env, &export, &exit_status, &exite);
	while (*mainstruct.exit == -1)
	{
		make_readline = ft_strjoin(ft_strjoin(ft_strdup(VERT "→ " BLEU),
					get_cwd(1)), ft_strdup(VIOLET " > " RESET));
		input = readline(make_readline);
		if (!input)
			stop_process(mainstruct, make_readline);
		add_history(input);
		check_prompt(input, mainstruct);
		free(make_readline);
	}
	free_tab(*mainstruct.env);
	free_tab(*mainstruct.export);
	clear_history();
	exit(*mainstruct.exit);
}
