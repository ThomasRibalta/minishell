/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_terminal2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 21:16:28 by toto              #+#    #+#             */
/*   Updated: 2024/06/11 21:16:52 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

int	my_wexitstatus(int status)
{
	return ((status & 0xff00) >> 8);
}

t_mainstruct	intialize_mainstruct(char ***env, char ***export,
		int *exit_status, int *exit)
{
	t_mainstruct	mainstruct;

	mainstruct.env = env;
	mainstruct.export = export;
	mainstruct.exit_status = exit_status;
	mainstruct.exit = exit;
	return (mainstruct);
}

void	aff_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		printf("%s\n", tab[i]);
		i++;
	}
}

void	stop_process(t_mainstruct mainstruct, char *make_readline)
{
	free(make_readline);
	free_tab(*mainstruct.env);
	free_tab(*mainstruct.export);
	clear_history();
	exit(0);
}
