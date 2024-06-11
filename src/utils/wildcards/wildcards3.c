/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:25:14 by toto              #+#    #+#             */
/*   Updated: 2024/06/10 18:33:15 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

int	count_files_with_prefix(DIR *dir, const char *prefix)
{
	int				n;
	struct dirent	*entry;
	size_t			prefix_len;

	n = 0;
	prefix_len = ft_strlen(prefix);
	while (1)
	{
		entry = readdir(dir);
		if (entry == NULL)
			break ;
		if (is_special_dir(entry->d_name))
			continue ;
		if (ft_strncmp(prefix, entry->d_name, prefix_len) == 0)
			n++;
	}
	return (n);
}

int	count_files_with_prefix_and_suffix(DIR *dir, const char *prefix,
		const char *suffix)
{
	int				n;
	struct dirent	*entry;
	size_t			prefix_len;
	size_t			suffix_len;
	size_t			name_len;

	n = 0;
	prefix_len = ft_strlen(prefix);
	suffix_len = ft_strlen(suffix);
	while (1)
	{
		entry = readdir(dir);
		if (entry == NULL)
			break ;
		if (is_special_dir(entry->d_name))
			continue ;
		name_len = ft_strlen(entry->d_name);
		if (name_len >= prefix_len + suffix_len && ft_strncmp(prefix,
				entry->d_name, prefix_len) == 0 && ft_strncmp(suffix,
				entry->d_name + name_len - suffix_len, suffix_len) == 0)
			n++;
	}
	return (n);
}

int	get_number_of_files(char *after, char *before)
{
	int	n;
	DIR	*dir;

	n = 0;
	dir = opendir(".");
	if (!dir)
		return (-1);
	if (!after && !before)
		n = count_all_files(dir);
	else if (after && !before)
		n = count_files_with_suffix(dir, after);
	else if (after && before)
		n = count_files_with_prefix_and_suffix(dir, before, after);
	else if (!after && before)
		n = count_files_with_prefix(dir, before);
	closedir(dir);
	return (n);
}

int	init_current_dir(int *n, int *i, char ***tab, DIR **dir)
{
	*n = get_number_of_files(NULL, NULL);
	if (*n == 0)
		return (0);
	*i = 0;
	*tab = malloc(sizeof(char *) * (*n + 1));
	if (!(*tab))
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	*dir = opendir(".");
	if (!(*dir))
	{
		perror("diropen");
		exit(EXIT_FAILURE);
	}
	return (1);
}
