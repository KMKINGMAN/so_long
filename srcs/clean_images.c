/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_images.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkurkar <mkurkar@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:52:17 by mkurkar           #+#    #+#             */
/*   Updated: 2025/01/30 16:52:59 by mkurkar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	cleanup_floor_images(t_game *game)
{
	int	i;

	i = 0;
	while (i < 10)
	{
		if (game->floor[i])
		{
			mlx_destroy_image(game->mlx, game->floor[i]);
			game->floor[i] = NULL;
		}
		i++;
	}
}

static void	cleanup_player_anims(t_game *game)
{
	int	i;

	i = 0;
	while (i < 6)
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
		i++;
	}
}

static void	cleanup_player_left_anims(t_game *game)
{
	int	i;

	i = 0;
	while (i < 6)
	{
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
		i++;
	}
}

static void	cleanup_collect_anims(t_game *game)
{
	int	i;

	i = 0;
	while (i < 8)
	{
		if (game->collect[i])
		{
			mlx_destroy_image(game->mlx, game->collect[i]);
			game->collect[i] = NULL;
		}
		i++;
	}
}

void	cleanup_scaled_images(t_game *game)
{
	cleanup_floor_images(game);
	cleanup_player_anims(game);
	cleanup_player_left_anims(game);
	cleanup_collect_anims(game);
}
