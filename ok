/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 14:10:30 by toto              #+#    #+#             */
/*   Updated: 2024/06/10 19:07:48 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/header/minishell.h"

int		g_global_sig;

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

int	main(int ac, char **av, char **env)
{
	int		i;
	char	**env2;
	char	**export;

	if (ac > 2 && !*av)
		return (0);
	init_signal();
	env2 = clone_env(env);
	init_terminal(env2);
	export = clone_env(env2);
	g_global_sig = 0;
	i = 0;
	start_terminal(env2, export, i);
	free_tab(env2);
	free_tab(export);
	return (0);
}
