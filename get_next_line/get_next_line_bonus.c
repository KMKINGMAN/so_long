/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkurkar <mkurkar@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:35:34 by mkurkar           #+#    #+#             */
/*   Updated: 2025/01/18 16:49:41 by mkurkar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*free_fd(char **files, int fd)
{
	if (files[fd] != NULL)
	{
		free(files[fd]);
		files[fd] = NULL;
	}
	return (NULL);
}

static char	*get_line(char **files, int fd)
{
	char	*buff;
	char	*res;
	int		i;

	if (!files[fd])
		return (NULL);
	buff = files[fd];
	i = ft_strchr(files[fd], '\n');
	if (i == -1)
	{
		if (ft_strlen(files[fd]) == 0)
			return (free_fd(files, fd));
		res = ft_strdup(files[fd]);
		free_fd(files, fd);
		return (res);
	}
	res = ft_substr(buff, 0, i + 1);
	if (res == NULL)
		return (free_fd(files, fd));
	files[fd] = ft_substr(buff, i + 1, (ft_strlen(buff) - i));
	free(buff);
	return (res);
}

static void	manage_buffer(char *buf, char **files, int fd, int *read_res)
{
	buf = (char *)malloc(BUFFER_SIZE + 1);
	if (buf == NULL)
	{
		free_fd(files, fd);
		return ;
	}
	*read_res = read(fd, buf, BUFFER_SIZE);
	if (*read_res == -1)
	{
		free(buf);
		free_fd(files, fd);
		return ;
	}
	buf[*read_res] = '\0';
	files[fd] = ft_strjoin(files[fd], buf);
	free(buf);
}

char	*get_next_line(int fd)
{
	char		*buf;
	static char	*files[FD_MAX];
	int			read_res;

	if (fd < 0 || fd > FD_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	if (files[fd] == NULL)
		files[fd] = ft_strdup("");
	read_res = 1;
	buf = NULL;
	while (read_res > 0)
	{
		if (ft_strchr(files[fd], '\n') == -1)
		{
			manage_buffer(buf, files, fd, &read_res);
			if (files[fd] == NULL)
				return (free_fd(files, fd));
		}
		else
			return (get_line(files, fd));
	}
	return (get_line(files, fd));
}
