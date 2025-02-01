/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkurkar <mkurkar@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 18:24:42 by mkurkar           #+#    #+#             */
/*   Updated: 2025/02/01 19:29:13 by mkurkar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	clean_newline(char *line)
{
	int	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
}

static char	**allocate_map(char *file, int *height)
{
	int		fd;
	char	*line;
	char	**lines;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (NULL);
	*height = 0;
	line = get_next_line(fd);
	while (line)
	{
		(*height)++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	lines = (char **)malloc(sizeof(char *) * (*height + 1));
	return (lines);
}

static int	validate_width(char **lines, int i, int width)
{
	if (i == 0)
		width = ft_strlen(lines[i]);
	else if ((int)ft_strlen(lines[i]) != width)
	{
		ft_printf("Error: Map line %d has different length\n", i + 1);
		return (0);
	}
	return (width);
}

char	**reading_map(int *width, char **lines, char *file)
{
	int		fd;
	char	*line;
	int		i;

	fd = open(file, O_RDONLY);
	i = 0;
	*width = 0;
	line = get_next_line(fd);
	while (line)
	{
		clean_newline(line);
		lines[i] = line;
		*width = validate_width(lines, i, *width);
		if (*width == 0)
		{
			free_map(lines, i);
			return (NULL);
		}
		i++;
		line = get_next_line(fd);
	}
	lines[i] = NULL;
	close(fd);
	return (lines);
}

char	**read_map(char *file, int *width, int *height)
{
	char	**lines;

	lines = allocate_map(file, height);
	if (!lines)
	{
		ft_printf("Error: Cannot open map file\n");
		return (NULL);
	}
	return (reading_map(width, lines, file));
}
