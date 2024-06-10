/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:54:12 by toto              #+#    #+#             */
/*   Updated: 2024/06/08 15:54:13 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

int	check_value(char *value)
{
	int	i;

	i = 0;
	if (value[i] == '-' || value[i] == '+')
		i++;
	while (value[i])
	{
		if (value[i] >= '0' || value[i] <= '9')
			i++;
		else
			return (0);
	}
	return (1);
}

void	exit_program(char **value, t_command *cmd)
{
	int	i;

	if (value[0] == NULL)
	{
		*cmd->mainstruct.exit = 0;
		return ;
	}
	if (len_tab(value) > 1)
	{
		printf("minishell: exit: too many arguments\n");
		*cmd->mainstruct.exit = 1;
		return ;
	}
	if (check_value(value[0]) == 0)
	{
		printf("minishell: exit: %s: numeric argument required\n", value[0]);
		*cmd->mainstruct.exit = 2;
		return ;
	}
	i = ft_atoi(value[0]);
	*cmd->mainstruct.exit = i;
	return ;
}
