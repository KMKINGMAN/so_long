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
	while ((line = get_next_line(fd)))
	{
		(*height)++;
		free(line);
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
/**
 * @brief Reads and parses the game map from a file
 * @param file Path to the map file
 * @param width Pointer to store map width
 * @param height Pointer to store map height
 * @return char** 2D array containing the map data
 * 
 * @details
 * - Reads map file line by line
 * - Allocates memory for map storage
 * - Validates map dimensions
 * - Returns NULL if any error occurs
 */
char	**read_map(char *file, int *width, int *height)
{
	int		fd;
	char	*line;
	char	**lines;
	int		i;

	lines = allocate_map(file, height);
	if (!lines)
	{
		ft_printf("Error: Cannot open map file\n");
		return (NULL);
	}
	fd = open(file, O_RDONLY);
	i = 0;
	*width = 0;
	while ((line = get_next_line(fd)))
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
	}
	lines[i] = NULL;
	close(fd);
	return (lines);
}
