/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkurkar <mkurkar@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 17:25:32 by mkurkar           #+#    #+#             */
/*   Updated: 2025/02/01 19:44:10 by mkurkar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	flood_fill(char **map_copy, int x, int y, t_counts *count)
{
	if (x < 0 || y < 0 || !map_copy[y] || !map_copy[y][x])
		return ;
	if (map_copy[y][x] == '1' || map_copy[y][x] == 'F')
		return ;
	if (map_copy[y][x] == 'C')
		count->collect--;
	else if (map_copy[y][x] == 'E')
		count->exit = 1;
	map_copy[y][x] = 'F';
	flood_fill(map_copy, x + 1, y, count);
	flood_fill(map_copy, x - 1, y, count);
	flood_fill(map_copy, x, y + 1, count);
	flood_fill(map_copy, x, y - 1, count);
}

static char	**copy_map(t_game *game)
{
	char	**map_copy;
	int		i;

	map_copy = (char **)malloc(sizeof(char *) * (game->map_height + 1));
	if (!map_copy)
		return (NULL);
	i = 0;
	while (i < game->map_height)
	{
		map_copy[i] = ft_strdup(game->map[i]);
		if (!map_copy[i])
		{
			free_map(map_copy, i);
			return (NULL);
		}
		i++;
	}
	map_copy[i] = NULL;
	return (map_copy);
}

static int	validate_path(t_game *game)
{
	char		**map_copy;
	t_counts	count;

	map_copy = copy_map(game);
	if (!map_copy)
		return (0);
	count.collect = game->collectibles_count;
	count.exit = 0;
	flood_fill(map_copy, game->player_x, game->player_y, &count);
	free_map(map_copy, game->map_height);
	if (count.collect > 0)
	{
		ft_printf("Error: Unable to reach all collectibles\n");
		return (0);
	}
	if (!count.exit)
	{
		ft_printf("Error: Unable to reach the exit\n");
		return (0);
	}
	return (1);
}

/**
 * Validates the game map according to specific
 *  rules and requirements.
 *
 * This function performs several validation
 *  checks on the game map:
 * 1. Checks if map borders are valid walls
 * 2. Validates internal map content and counts
 *  elements
 * 3. Verifies required element counts
 *  (player, exit, collectibles)
 * 4. Validates if there exists a valid path to
 *  collect all items and reach exit
 *
 * @param {t_game*} game - Pointer to game structure
 *  containing map data
 *
 * @returns {int} 1 if map is valid, 0 if any
 *  validation fails
 *
 * @example
 * t_game game;
 * // Initialize game structure with map data
 * game.map = [
 *   "1111111",
 *   "1P0C001",
 *   "100E001",
 *   "1111111"
 * ];
 * game.map_width = 7;
 * game.map_height = 4;
 *
 * if (validate_map(&game)) {
 *     // Map is valid, proceed with game
 * } else {
 *     // Handle invalid map error
 * }
 *
 * @throws Can print error messages to standard
 * output if validation fails
 * @note Successful validation prints map statistics
 * including dimensions and collectible count
 */
int	validate_map(t_game *game)
{
	t_counts	count;

	count = (t_counts){0, 0, 0};
	if (!check_map_borders(game))
		return (0);
	if (!check_map_content(game, &count))
		return (0);
	if (!validate_counts(game, count))
		return (0);
	if (!validate_path(game))
		return (0);
	ft_printf("Map validation successful:\n");
	ft_printf("- Size: %dx%d\n", game->map_width, game->map_height);
	ft_printf("- Collectibles: %d\n", count.collect);
	return (1);
}

/**
 * @brief Frees the allocated map memory
 * @param map 2D array to free
 * @param height Number of rows in the map
 */
void	free_map(char **map, int height)
{
	int	i;

	if (!map)
		return ;
	i = 0;
	while (i < height)
	{
		free(map[i]);
		i++;
	}
	free(map);
}
