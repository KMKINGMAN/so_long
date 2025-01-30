/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkurkar <mkurkar@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 17:25:32 by mkurkar           #+#    #+#             */
/*   Updated: 2025/01/30 17:08:22 by mkurkar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	flood_fill(char **map_copy, int x, int y, t_counts *count)
{
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
