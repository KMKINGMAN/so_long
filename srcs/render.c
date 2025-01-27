/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkurkar <mkurkar@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 17:16:42 by mkurkar           #+#    #+#             */
/*   Updated: 2025/01/27 20:05:47 by mkurkar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file render.c
 * @brief Rendering functions for the so_long game
 */

#include "so_long.h"
#include <time.h>

/**
 * @brief Copies a sprite to the buffer image with transparency handling
 *
 * @param dst     Destination buffer image
 * @param src_ptr Source sprite pointer
 * @param x       X coordinate in destination
 * @param y       Y coordinate in destination
 *
 * @details
 * Copies a 32x32 sprite to the buffer while handling transparency.
 * Only non-transparent pixels (alpha != 0) are copied.
 */
static void	put_img_to_img_part(t_img *dest, t_img *sprite, t_pos pos)
{
	int	sprite_pos;
	int	dest_pos;

	sprite_pos = pos.col * (sprite->bpp / 8) + pos.row * sprite->line_len;
	dest_pos = (pos.dest_x + pos.col) * (dest->bpp / 8) + (pos.dest_y + pos.row)
		* dest->line_len;
	if ((*(unsigned int *)(sprite->addr + sprite_pos) & 0xFF000000) == 0)
		*(unsigned int *)(dest->addr
				+ dest_pos) = *(unsigned int *)(sprite->addr + sprite_pos);
}

void	put_img_to_img(t_img *dest, void *sprite_ptr, int dest_x, int dest_y)
{
	t_img	sprite;
	t_pos	pos;

	sprite.img_ptr = sprite_ptr;
	sprite.addr = mlx_get_data_addr(sprite.img_ptr, &sprite.bpp,
			&sprite.line_len, &sprite.endian);
	pos.row = 0;
	pos.dest_x = dest_x;
	pos.dest_y = dest_y;
	while (pos.row < 32)
	{
		pos.col = 0;
		while (pos.col < 32)
		{
			put_img_to_img_part(dest, &sprite, pos);
			pos.col++;
		}
		pos.row++;
	}
}

/**
 * @brief Initializes the rendering buffer
 * @param game Pointer to game structure
 *
 * @details
 * Creates an off-screen buffer for rendering the game frame.
 * Buffer size matches the window dimensions.
 * Used for double buffering to prevent flickering.
 */
void	init_buffer(t_game *game)
{
	int	win_width;
	int	win_height;

	win_width = game->map_width * game->tile_size;
	win_height = game->map_height * game->tile_size;
	game->buffer.img_ptr = mlx_new_image(game->mlx, win_width, win_height);
	game->buffer.addr = mlx_get_data_addr(game->buffer.img_ptr,
			&game->buffer.bpp, &game->buffer.line_len, &game->buffer.endian);
}

/**
 * @brief Handles game animation frames and state updates
 *
 * @param game Pointer to game structure
 * @return int Always returns 0
 *
 * @details
 * - Updates animation frames every 100 delay ticks
 * - Manages player running/idle state transitions
 * - Triggers frame rendering
 */
int	animate(t_game *game)
{
	static struct timespec	last_time;
	struct timespec			current_time;
	double					elapsed;

	clock_gettime(CLOCK_MONOTONIC, &current_time);
	elapsed = (current_time.tv_sec - last_time.tv_sec) * 1000.0
		+ (current_time.tv_nsec - last_time.tv_nsec) / 1000000.0;
	if (elapsed >= 100.0 || last_time.tv_sec == 0)
	{
		game->frame = (game->frame + 1) % 6;
		last_time = current_time;
		if (game->is_running)
		{
			game->run_counter++;
			if (game->run_counter >= 20)
			{
				game->is_running = 0;
				game->run_counter = 0;
			}
		}
	}
	render_frame(game);
	return (0);
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

/**
 * @brief Handles window expose events
 * @param game Pointer to game structure
 * @return int Result of render_frame
 *
 * @details
 * Called when window needs to be redrawn,
 * typically after being uncovered or restored.
 */
int	expose_hook(t_game *game)
{
	return (render_frame(game));
}
