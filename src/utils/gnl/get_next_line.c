/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 12:52:24 by thoribal          #+#    #+#             */
/*   Updated: 2024/06/11 17:41:48 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char	*get_next_end(int fd, char *str)
{
	char	*buffer;
	int		b;

	buffer = malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	b = 1;
	while (b != 0 && !ft_strchr_gnl(str, '\n'))
	{
		b = read(fd, buffer, BUFFER_SIZE);
		if (b < 0)
		{
			free(buffer);
			return (NULL);
		}
		buffer[b] = '\0';
		str = ft_strjoin_gnl(str, buffer);
	}
	free(buffer);
	return (str);
}

char	*get_line(char *s)
{
	int		i;
	char	*str;

	i = 0;
	if (!s[i])
		return (NULL);
	while (s[i] && s[i] != '\n')
		i++;
	str = ft_substr_gnl(s, 0, i + 1);
	if (!str)
		return (NULL);
	return (str);
}

char	*save_next(char *s)
{
	int		i;
	char	*str;

	i = 0;
	while (s[i] && s[i] != '\n')
		i++;
	if (!s[i])
	{
		free(s);
		return (NULL);
	}
	str = ft_substr_gnl(s, i + 1, ft_strlen_gnl(s) - i - 1);
	if (!str)
		return (NULL);
	free(s);
	return (str);
}

char	*get_next_line(int fd, int i)
{
	char		*line;
	static char	*s;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (i == 1)
	{
		free(s);
		return (NULL);
	}
	s = get_next_end(fd, s);
	if (!s)
		return (NULL);
	line = get_line(s);
	s = save_next(s);
	return (line);
}
