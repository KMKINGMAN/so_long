/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkurkar <mkurkar@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 18:24:34 by mkurkar           #+#    #+#             */
/*   Updated: 2025/01/30 16:55:34 by mkurkar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	cleanup_mlx_images(t_game *game)
{
	if (game->buffer.img_ptr)
	{
		mlx_destroy_image(game->mlx, game->buffer.img_ptr);
		game->buffer.img_ptr = NULL;
	}
	cleanup_scaled_images(game);
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
}

static void	cleanup_window(t_game *game)
{
	if (game->win)
	{
		mlx_clear_window(game->mlx, game->win);
		mlx_destroy_window(game->mlx, game->win);
		game->win = NULL;
	}
}

static void	cleanup_map_data(t_game *game)
{
	int	i;

	if (game->map)
	{
		i = 0;
		while (i < game->map_height)
		{
			free(game->map[i]);
			i++;
		}
		free(game->map);
		game->map = NULL;
	}
	if (game->floor_types)
	{
		i = 0;
		while (i < game->map_height)
		{
			free(game->floor_types[i]);
			i++;
		}
		free(game->floor_types);
		game->floor_types = NULL;
	}
}

void	cleanup_game(t_game *game)
{
	if (!game)
		return ;
	if (game->mlx)
	{
		cleanup_mlx_images(game);
		cleanup_window(game);
		if (game->mlx)
		{
			mlx_destroy_display(game->mlx);
			free(game->mlx);
			game->mlx = NULL;
		}
	}
	cleanup_map_data(game);
}

void	handle_error(t_game *game, const char *message)
{
	ft_printf("Error: %s\n", message);
	cleanup_game(game);
	exit(1);
}
