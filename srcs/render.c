/**
 * @file render.c
 * @brief Rendering functions for the so_long game
 */

#include "../includes/so_long.h"

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
void put_img_to_img(t_img *dst, void *src_ptr, int x, int y)
{
	t_img src;
	int i;
	int j;

	src.img_ptr = src_ptr;
	src.addr = mlx_get_data_addr(src.img_ptr, &src.bpp, &src.line_len, &src.endian);

	i = 0;
	while (i < 32)
	{
		j = 0;
		while (j < 32)
		{
			int src_i = j * (src.bpp / 8) + i * src.line_len;
			int dst_i = (x + j) * (dst->bpp / 8) + (y + i) * dst->line_len;
			unsigned int color;

			color = *(unsigned int *)(src.addr + src_i);
			if ((color & 0xFF000000) == 0)
				*(unsigned int *)(dst->addr + dst_i) = color;
			j++;
		}
		i++;
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
void init_buffer(t_game *game)
{
	int win_width = game->map_width * game->tile_size;
	int win_height = game->map_height * game->tile_size;

	game->buffer.img_ptr = mlx_new_image(game->mlx, win_width, win_height);
	game->buffer.addr = mlx_get_data_addr(game->buffer.img_ptr, &game->buffer.bpp,
										  &game->buffer.line_len, &game->buffer.endian);
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
int animate(t_game *game)
{
	static int delay = 0;

	delay++;
	if (delay > 100)
	{
		delay = 0;
		game->frame = (game->frame + 1) % 6;

		if (game->is_running)
		{
			game->run_counter++;
			if (game->run_counter >= 100) // After 100 frames of running
			{
				game->is_running = 0;  // Set to idle
				game->run_counter = 0; // Reset counter
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
int render_frame(t_game *game)
{
	int i = 0;
	int j = 0;

	// Clear buffer by setting all pixels to 0
	memset(game->buffer.addr, 0,
		   game->map_width * game->tile_size * game->map_height * game->tile_size *
			   (game->buffer.bpp / 8));

	// First pass: Draw floor and exit
	while (game->map[i])
	{
		j = 0;
		while (game->map[i][j])
		{
			int x = j * game->tile_size;
			int y = i * game->tile_size;
			put_img_to_img(&game->buffer, game->floor[game->floor_types[i][j]], x, y);
			if (game->map[i][j] == 'E')
				put_img_to_img(&game->buffer, game->exit, x, y);
			j++;
		}
		i++;
	}

	// Second pass: Draw walls and entities
	i = 0;
	while (game->map[i])
	{
		j = 0;
		while (game->map[i][j])
		{
			int x = j * game->tile_size;
			int y = i * game->tile_size;

			if (game->map[i][j] == '1')
				put_img_to_img(&game->buffer, game->wall, x, y);
			else if (game->map[i][j] == 'P')
			{
				void *sprite;
				if (game->facing_right)
				{
					sprite = game->is_running ? game->player_run[game->frame % 6] : game->player_idle[game->frame % 6];
				}
				else
				{
					sprite = game->is_running ? game->player_run_left[game->frame % 6] : game->player_idle_left[game->frame % 6];
				}
				put_img_to_img(&game->buffer, sprite, x, y);
			}
			else if (game->map[i][j] == 'C')
				put_img_to_img(&game->buffer, game->collect[game->frame % 8], x, y);
			j++;
		}
		i++;
	}

	// Put the buffer to window
	mlx_put_image_to_window(game->mlx, game->win, game->buffer.img_ptr, 0, 0);

	// Draw move counter
	char moves_str[32];
	snprintf(moves_str, sizeof(moves_str), "Moves: %d", game->moves_count);
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
int expose_hook(t_game *game)
{
	return (render_frame(game));
}
