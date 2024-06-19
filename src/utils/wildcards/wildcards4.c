/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:28:16 by toto              #+#    #+#             */
/*   Updated: 2024/06/11 21:43:46 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

char	**get_current_file(void)
{
	char			**tab;
	int				n;
	int				i;
	DIR				*dir;
	struct dirent	*entry;

	if (!init_current_dir(&n, &i, &tab, &dir))
		return (NULL);
	while (1)
	{
		entry = readdir(dir);
		if (entry == NULL)
			break ;
		if ((entry->d_name[0] == '.' && (entry->d_name[1] == '\0'
					|| (entry->d_name[1] == '.' && entry->d_name[2] == '\0')))
			|| entry->d_name[0] == '.')
			continue ;
		tab[i] = ft_strdup(entry->d_name);
		i++;
	}
	closedir(dir);
	tab[i] = NULL;
	return (tab);
}

char	**get_current_file_after(char *str)
{
	char			**tab;
	int				n;
	int				i;
	DIR				*dir;
	struct dirent	*entry;

	if (!init_current_dir(&n, &i, &tab, &dir))
		return (NULL);
	while (1)
	{
		entry = readdir(dir);
		if (entry == NULL)
			break ;
		if ((entry->d_name[0] == '.' && (entry->d_name[1] == '\0'
					|| (entry->d_name[1] == '.' && entry->d_name[2] == '\0')))
			|| entry->d_name[0] == '.')
			continue ;
		if (strncmp(str, entry->d_name + ft_strlen(entry->d_name)
				- ft_strlen(str), ft_strlen(str)) == 0)
			tab[i++] = ft_strdup(entry->d_name);
	}
	closedir(dir);
	tab[i] = NULL;
	return (tab);
}

char	**get_current_file_after_before(char *str, char *str2)
{
	char			**tab;
	int				n;
	int				i;
	DIR				*dir;
	struct dirent	*entry;

	if (!init_current_dir(&n, &i, &tab, &dir))
		return (NULL);
	while (1)
	{
		entry = readdir(dir);
		if (entry == NULL)
			break ;
		if ((entry->d_name[0] == '.' && (entry->d_name[1] == '\0'
					|| (entry->d_name[1] == '.' && entry->d_name[2] == '\0')))
			|| entry->d_name[0] == '.')
			continue ;
		if (ft_strncmp(str2, entry->d_name, ft_strlen(str2)) == 0
			&& ft_strncmp(str, entry->d_name + ft_strlen(entry->d_name)
				- ft_strlen(str), ft_strlen(str)) == 0)
			tab[i++] = ft_strdup(entry->d_name);
	}
	closedir(dir);
	tab[i] = NULL;
	return (tab);
}

int	only_char(char *str, char c)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] != c)
			return (i);
		i++;
	}
	return (i);
}
