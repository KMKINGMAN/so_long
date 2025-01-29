/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkurkar <mkurkar@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 17:25:32 by mkurkar           #+#    #+#             */
/*   Updated: 2025/01/29 03:24:36 by mkurkar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

/**
 * @brief Simple map validation check for basic requirements
 * @param game Pointer to game structure
 * @return int 1 if valid, 0 if invalid
 */
static int check_map_borders(t_game *game)
{
	int j;

	if (game->map_height < 3 || game->map_width < 3)
		return (ft_printf("Error: Map too small\n") && 0);
	j = -1;
	while (++j < game->map_width)
		if (game->map[0][j] != '1' || game->map[game->map_height - 1][j] != '1')
			return (ft_printf("Error: Map not surrounded by walls\n") && 0);
	return (1);
}

static int check_elements(t_game *game, int i, int j, t_counts *count)
{
	char c;

	c = game->map[i][j];
	if (c == 'P')
	{
		count->player++;
		game->player_x = j;
		game->player_y = i;
	}
	else if (c == 'E')
	{
		count->exit++;
		game->exit_x = j;
		game->exit_y = i;
	}
	else if (c == 'C')
		count->collect++;
	else if (c != '0' && c != '1')
		return (ft_printf("Error: Invalid character in map\n") && 0);
	return (1);
}

static int validate_counts(t_game *game, t_counts count)
{
	if (count.player != 1)
		return (ft_printf("Error: Must have exactly one player\n") && 0);
	if (count.exit != 1)
		return (ft_printf("Error: Must have exactly one exit\n") && 0);
	if (count.collect < 1)
		return (ft_printf("Error: Must have at least one collectible\n") && 0);
	game->collectibles_count = count.collect;
	return (1);
}

static int check_map_content(t_game *game, t_counts *count)
{
	int i;
	int j;

	i = -1;
	while (++i < game->map_height)
	{
		if ((int)ft_strlen(game->map[i]) != game->map_width)
			return (ft_printf("Error: Map not rectangular\n") && 0);
		if (game->map[i][0] != '1' || game->map[i][game->map_width - 1] != '1')
			return (ft_printf("Error: Map not surrounded by walls\n") && 0);
		j = -1;
		while (++j < game->map_width)
			if (!check_elements(game, i, j, count))
				return (0);
	}
	return (1);
}

static void flood_fill(char **map_copy, int x, int y, t_counts *count)
{
	if (map_copy[y][x] == '1' || map_copy[y][x] == 'F')
		return;

	if (map_copy[y][x] == 'C')
		count->collect--;
	else if (map_copy[y][x] == 'E')
		count->exit = 1; // Mark that we reached the exit

	map_copy[y][x] = 'F'; // Mark as filled

	flood_fill(map_copy, x + 1, y, count);
	flood_fill(map_copy, x - 1, y, count);
	flood_fill(map_copy, x, y + 1, count);
	flood_fill(map_copy, x, y - 1, count);
}

static char **copy_map(t_game *game)
{
	char **map_copy;
	int i;

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

static int validate_path(t_game *game)
{
	char **map_copy;
	t_counts count;

	map_copy = copy_map(game);
	if (!map_copy)
		return (0);

	// Initialize count structure for path validation
	count.collect = game->collectibles_count;
	count.exit = 0; // Use this to track if exit is reached

	flood_fill(map_copy, game->player_x, game->player_y, &count);
	free_map(map_copy, game->map_height);

	// Check if all collectibles were reached
	if (count.collect > 0)
	{
		ft_printf("Error: Unable to reach all collectibles\n");
		return (0);
	}

	// Check if exit was reached
	if (!count.exit)
	{
		ft_printf("Error: Unable to reach the exit\n");
		return (0);
	}

	return (1);
}

int validate_map(t_game *game)
{
	t_counts count;

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
void free_map(char **map, int height)
{
	int i;

	if (!map)
		return;
	i = 0;
	while (i < height)
	{
		free(map[i]);
		i++;
	}
	free(map);
}
