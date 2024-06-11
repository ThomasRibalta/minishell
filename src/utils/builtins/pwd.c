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
		return ("");
	tmp = cwd;
	if (i == 1 && ft_strlen(cwd) >= ft_strlen(getenv("HOME")))
	{
		tmp = ft_strjoin(ft_strdup("~"), ft_strdup(cwd
					+ ft_strlen(getenv("HOME"))));
		free(cwd);
	}
	return (tmp);
}

char	*pwd(int i)
{
	char	*cwd;

	cwd = get_cwd(i);
	if (cwd[0] == '\0')
	{
		return ("pwd: error retrieving current directory:"
			" getcwd: cannot access parent directories:"
			" No such file or directory\n");
	}
	else
		return (cwd);
}
