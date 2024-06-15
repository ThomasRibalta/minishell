/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:02:06 by toto              #+#    #+#             */
/*   Updated: 2024/06/08 16:02:07 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

void print_sorted_env(char **env)
{
	int		i;
	int		j;
	char	*tmp;

	i = -1;
	while (env[++i] != NULL)
	{
		j = i + 1;
		while (env[j] != NULL)
		{
			if (ft_strcmp(env[i], env[j]) > 0)
			{
				tmp = env[i];
				env[i] = env[j];
				env[j] = tmp;
			}
			j++;
		};
	}
	i = 0;
	while (env[i] != NULL)
	{
		printf("declare -x %s\n", env[i]);
		i++;
	}
}

void	print_env(char **env, int nb)
{
	int	i;

	i = 0;
	if (nb == 1)
		print_sorted_env(env);
	else
	{
		while (env[i] != NULL)
		{
			printf("%s\n", env[i]);
			i++;
		}
	}
}
