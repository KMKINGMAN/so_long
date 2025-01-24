#include "../includes/so_long.h"

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
char **read_map(char *file, int *width, int *height)
{
	int fd;
	char *line;
	char **lines;
	int i;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		ft_printf("Error: Cannot open map file\n");
		return (NULL);
	}
	*height = 0;
	while ((line = get_next_line(fd)))
	{
		(*height)++;
		free(line);
	}
	close(fd);
	lines = (char **)malloc(sizeof(char *) * (*height + 1));
	fd = open(file, O_RDONLY);
	i = 0;
	while ((line = get_next_line(fd)))
	{
		// Remove newline character if present
		int len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
			
		lines[i] = line;
		if (i == 0)
			*width = strlen(line);
		else if ((int)strlen(line) != *width)
		{
			ft_printf("Error: Map line %d has different length\n", i + 1);
			free_map(lines, i);
			return NULL;
		}
		i++;
	}
	lines[i] = NULL;
	close(fd);
	*width = ft_strlen(lines[0]);
	return (lines);
}
