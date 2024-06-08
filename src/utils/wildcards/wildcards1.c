/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:21:12 by toto              #+#    #+#             */
/*   Updated: 2024/06/08 16:24:15 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

char	**fusionner_tableaux(char **tab1, char **tab2, int i)
{
	int		taille_total;
	char	**tableau_fusionne;
	int		j;

	taille_total = len_tab(tab1) + len_tab(tab2) + 1;
	j = -1;
	tableau_fusionne = (char **)malloc(taille_total * sizeof(char *));
	if (tableau_fusionne == NULL)
		exit(EXIT_FAILURE);
	while (++j < i)
		tableau_fusionne[j] = ft_strdup(tab1[j]);
	j = -1;
	while (++j < len_tab(tab2))
		tableau_fusionne[i + j] = ft_strdup(tab2[j]);
	j = i;
	while (tab1[++j])
		tableau_fusionne[len_tab(tab2) + j - 1] = ft_strdup(tab1[j]);
	tableau_fusionne[taille_total - 1] = NULL;
	free_tab(tab1);
	free_tab(tab2);
	return (tableau_fusionne);
}

int	is_special_dir(const char *name)
{
	if (name[0] == '.' && (name[1] == '\0' || (name[1] == '.'
				&& name[2] == '\0')))
		return (1);
	return (0);
}

int	count_all_files(DIR *dir)
{
	int				n;
	struct dirent	*entry;

	n = 0;
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (is_special_dir(entry->d_name))
			continue ;
		n++;
		entry = readdir(dir);
	}
	return (n);
}

int	count_files_with_suffix(DIR *dir, const char *suffix)
{
	int				n;
	struct dirent	*entry;
	size_t			suffix_len;
	size_t			name_len;

	n = 0;
	suffix_len = ft_strlen(suffix);
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (is_special_dir(entry->d_name))
			continue ;
		name_len = ft_strlen(entry->d_name);
		if (name_len >= suffix_len && ft_strncmp(suffix, entry->d_name
				+ name_len - suffix_len, suffix_len) == 0)
			n++;
		entry = readdir(dir);
	}
	return (n);
}
