/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:56:14 by toto              #+#    #+#             */
/*   Updated: 2024/06/11 21:30:49 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

char	*get_cwd(int i)
{
	char	*cwd;
	char	*tmp;

	cwd = malloc(1024);
	if (!cwd)
		return ("Erreur");
	if (!getcwd(cwd, 1024))
	{
		free(cwd);
		return (ft_strdup(""));
	}
	tmp = cwd;
	if (i == 1 && ft_strlen(cwd) >= ft_strlen(getenv("HOME")))
	{
		tmp = ft_strjoin(ft_strdup("~"), ft_strdup(cwd
					+ ft_strlen(getenv("HOME"))));
		free(cwd);
	}
	if (tmp == NULL || tmp[0] == '\0')
	{
		return (NULL);
	}
	return (tmp);
}

char	*pwd(int i)
{
	char	*cwd;

	cwd = get_cwd(i);
	if (cwd[0] == '\0')
	{
		free(cwd);
		return (ft_strdup("pwd: error retrieving current directory:"
				" getcwd: cannot access parent directories:"
				" No such file or directory"));
	}
	else
		return (cwd);
}
