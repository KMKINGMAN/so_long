/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_frame.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkurkar <mkurkar@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:57:54 by mkurkar           #+#    #+#             */
/*   Updated: 2025/01/30 16:58:32 by mkurkar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	draw_floor_and_exit(t_game *game)
{
	int	i;
	int	j;
	int	x;
	int	y;

	i = 0;
	while (game->map[i])
	{
		j = 0;
		while (game->map[i][j])
		{
			x = j * game->tile_size;
			y = i * game->tile_size;
			put_img_to_img(&game->buffer, game->floor[game->floor_types[i][j]],
				x, y);
			if (game->map[i][j] == 'E' && game->can_exit)
				put_img_to_img(&game->buffer, game->exit, x, y);
			j++;
		}
		i++;
	}
}

static void	draw_player(t_game *game, int x, int y)
{
	void	*sprite;

	if (game->facing_right)
	{
		if (game->is_running)
			sprite = game->player_run[game->frame % 6];
		else
			sprite = game->player_idle[game->frame % 6];
	}
	else
	{
		if (game->is_running)
			sprite = game->player_run_left[game->frame % 6];
		else
			sprite = game->player_idle_left[game->frame % 6];
	}
	put_img_to_img(&game->buffer, sprite, x, y);
}

static void	draw_entities(t_game *game)
{
	int	i;
	int	j;
	int	x;
	int	y;

	i = 0;
	while (game->map[i])
	{
		j = 0;
		while (game->map[i][j])
		{
			x = j * game->tile_size;
			y = i * game->tile_size;
			if (game->map[i][j] == '1')
				put_img_to_img(&game->buffer, game->wall, x, y);
			else if (game->map[i][j] == 'P')
				draw_player(game, x, y);
			else if (game->map[i][j] == 'C')
				put_img_to_img(&game->buffer, game->collect[game->frame % 8], x,
					y);
			j++;
		}
		i++;
	}
}

/**
 * @brief Renders a complete frame of the game
 * @param game Pointer to game structure
 * @return int Always returns 0
 *
 * @details Rendering order:
 * 1. Clear buffer
 * 2. Draw floor tiles
 * 3. Draw exit
 * 4. Draw walls
 * 5. Draw player with direction/animation
 * 6. Draw collectibles
 * 7. Draw UI elements
 */
int	render_frame(t_game *game)
{
	char	moves_str[32];

	ft_memset(game->buffer.addr, 0, game->map_width * game->tile_size
		* game->map_height * game->tile_size * (game->buffer.bpp / 8));
	draw_floor_and_exit(game);
	draw_entities(game);
	mlx_put_image_to_window(game->mlx, game->win, game->buffer.img_ptr, 0, 0);
	ft_snprintf(moves_str, sizeof(moves_str), "Moves: %d", game->moves_count);
	mlx_string_put(game->mlx, game->win, 10, 20, 0xFFFFFF, moves_str);
	return (0);
}
