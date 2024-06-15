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

void	clean_exit(void)
{
	char	*tmp;
	int		pipefd[2];
	int		saved_stdin;

	if (pipe(pipefd) == -1)
		exit(EXIT_FAILURE);
	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin == -1)
		exit(EXIT_FAILURE);
	tmp = get_next_line(saved_stdin, 0);
	while (tmp)
	{
		free(tmp);
		tmp = get_next_line(saved_stdin, 0);
	}
	free(tmp);
	tmp = get_next_line(saved_stdin, 1);
	free(tmp);
	write(pipefd[1], "", 1);
	close(pipefd[0]);
	close(pipefd[1]);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
}
