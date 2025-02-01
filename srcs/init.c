/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkurkar <mkurkar@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 18:24:51 by mkurkar           #+#    #+#             */
/*   Updated: 2025/02/01 19:20:08 by mkurkar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

/**
 * @brief Initializes random floor tile variations
 * @param game Pointer to game structure
 *
 * @details
 * Creates a 2D array of floor type indices.
 * Each position gets a random floor variant (0-9)
 * for visual variety in the game.
 */
void	init_floor_types(t_game *game)
{
	int	i;
	int	j;

	i = 0;
	game->floor_types = (unsigned char **)malloc(sizeof(unsigned char *)
			* game->map_height);
	while (i < game->map_height)
	{
		j = 0;
		game->floor_types[i] = (unsigned char *)malloc(sizeof(unsigned char)
				* game->map_width);
		while (j < game->map_width)
		{
			game->floor_types[i][j] = (unsigned char)(rand() % 10);
			j++;
		}
		i++;
	}
}

/**
 * @brief Locates and sets the player's starting position
 * @param game Pointer to game structure
 *
 * @details
 * Scans the map for 'P' character and sets
 * player_x and player_y coordinates accordingly.
 */
void	init_player_position(t_game *game)
{
	int	i;
	int	j;

	i = 0;
	while (game->map[i])
	{
		j = 0;
		while (game->map[i][j])
		{
			if (game->map[i][j] == 'P')
			{
				game->player_x = j;
				game->player_y = i;
				return ;
			}
			j++;
		}
		i++;
	}
}

/**
 * @brief Initializes collectible items and exit
 * @param game Pointer to game structure
 *
 * @details
 * - Counts total collectibles ('C' in map)
 * - Locates exit position ('E' in map)
 * - Initializes completion tracking
 */
void	init_collectibles(t_game *game)
{
	int	i;
	int	j;

	i = 0;
	game->collectibles_count = 0;
	while (game->map[i])
	{
		j = 0;
		while (game->map[i][j])
		{
			if (game->map[i][j] == 'C')
				game->collectibles_count++;
			else if (game->map[i][j] == 'E')
			{
				game->exit_x = j;
				game->exit_y = i;
			}
			j++;
		}
		i++;
	}
	game->can_exit = 0;
}
