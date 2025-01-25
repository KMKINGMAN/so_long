/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkurkar <mkurkar@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 18:24:34 by mkurkar           #+#    #+#             */
/*   Updated: 2025/01/25 18:24:34 by mkurkar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void cleanup_mlx_ptr(t_game *game)
{
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
		game->mlx = NULL;
	}
}

static void cleanup_scaled_images(t_game *game)
{
	int i;

	// Cleanup scaled floor images
	for (i = 0; i < 10; i++)
	{
		if (game->floor[i])
		{
			mlx_destroy_image(game->mlx, game->floor[i]);
			game->floor[i] = NULL;
		}
	}

	// Cleanup scaled player animations
	for (i = 0; i < 6; i++)
	{
		if (game->player_idle[i])
		{
			mlx_destroy_image(game->mlx, game->player_idle[i]);
			game->player_idle[i] = NULL;
		}
		if (game->player_run[i])
		{
			mlx_destroy_image(game->mlx, game->player_run[i]);
			game->player_run[i] = NULL;
		}
		if (game->player_idle_left[i])
		{
			mlx_destroy_image(game->mlx, game->player_idle_left[i]);
			game->player_idle_left[i] = NULL;
		}
		if (game->player_run_left[i])
		{
			mlx_destroy_image(game->mlx, game->player_run_left[i]);
			game->player_run_left[i] = NULL;
		}
	}

	// Cleanup collectible animations
	for (i = 0; i < 8; i++)
	{
		if (game->collect[i])
		{
			mlx_destroy_image(game->mlx, game->collect[i]);
			game->collect[i] = NULL;
		}
	}
}

void cleanup_game(t_game *game)
{
	int i;

	if (!game)
		return;

	// First cleanup all MLX images
	if (game->mlx)
	{
		if (game->buffer.img_ptr)
		{
			mlx_destroy_image(game->mlx, game->buffer.img_ptr);
			game->buffer.img_ptr = NULL;
		}

		cleanup_scaled_images(game);

		// Cleanup single images
		if (game->wall)
		{
			mlx_destroy_image(game->mlx, game->wall);
			game->wall = NULL;
		}
		if (game->exit)
		{
			mlx_destroy_image(game->mlx, game->exit);
			game->exit = NULL;
		}

		// Cleanup window
		if (game->win)
		{
			mlx_clear_window(game->mlx, game->win);
			mlx_destroy_window(game->mlx, game->win);
			game->win = NULL;
		}

		// Finally cleanup MLX itself
		mlx_destroy_display(game->mlx);
		free(game->mlx);
		game->mlx = NULL;
	}

	// Cleanup map data
	if (game->map)
	{
		for (i = 0; i < game->map_height; i++)
			free(game->map[i]);
		free(game->map);
		game->map = NULL;
	}

	// Cleanup floor types
	if (game->floor_types)
	{
		for (i = 0; i < game->map_height; i++)
			free(game->floor_types[i]);
		free(game->floor_types);
		game->floor_types = NULL;
	}

	// Finally cleanup MLX
	cleanup_mlx_ptr(game);
}

void handle_error(t_game *game, const char *message)
{
	ft_printf("Error: %s\n", message);
	cleanup_game(game);
	exit(1);
}
