#include "../includes/so_long.h"

/**
 * @brief Simple map validation check for basic requirements
 * @param game Pointer to game structure
 * @return int 1 if valid, 0 if invalid
 */
int validate_map(t_game *game)
{
	int i, j;
	int player = 0, exit = 0, collect = 0;

	// Check dimensions
	if (game->map_height < 3 || game->map_width < 3)
		return (ft_printf("Error: Map too small\n") && 0);

	// Check first and last rows are walls
	for (j = 0; j < game->map_width; j++)
	{
		if (game->map[0][j] != '1' || game->map[game->map_height - 1][j] != '1')
			return (ft_printf("Error: Map not surrounded by walls\n") && 0);
	}

	// Check each row
	for (i = 0; i < game->map_height; i++)
	{
		// Check row length
		if ((int)strlen(game->map[i]) != game->map_width)
			return (ft_printf("Error: Map not rectangular\n") && 0);

		// Check side walls
		if (game->map[i][0] != '1' || game->map[i][game->map_width - 1] != '1')
			return (ft_printf("Error: Map not surrounded by walls\n") && 0);

		// Count elements
		for (j = 0; j < game->map_width; j++)
		{
			char c = game->map[i][j];
			if (c == 'P')
			{
				player++;
				game->player_x = j;
				game->player_y = i;
			}
			else if (c == 'E')
			{
				exit++;
				game->exit_x = j;
				game->exit_y = i;
			}
			else if (c == 'C')
				collect++;
			else if (c != '0' && c != '1')
				return (ft_printf("Error: Invalid character in map\n") && 0);
		}
	}

	// Validate counts
	if (player != 1)
		return (ft_printf("Error: Must have exactly one player\n") && 0);
	if (exit != 1)
		return (ft_printf("Error: Must have exactly one exit\n") && 0);
	if (collect < 1)
		return (ft_printf("Error: Must have at least one collectible\n") && 0);

	// Store collectible count
	game->collectibles_count = collect;

	ft_printf("Map validation successful:\n");
	ft_printf("- Size: %dx%d\n", game->map_width, game->map_height);
	ft_printf("- Collectibles: %d\n", collect);
	return (1);
}

/**
 * @brief Frees the allocated map memory
 * @param map 2D array to free
 * @param height Number of rows in the map
 */
void free_map(char **map, int height)
{
	if (!map)
		return;
	for (int i = 0; i < height; i++)
		free(map[i]);
	free(map);
}
