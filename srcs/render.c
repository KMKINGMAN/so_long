/**
 * @file render.c
 * @brief Rendering functions for the so_long game
 */

#include "so_long.h"

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
 * @brief Simple animation handler with controlled speed
 */
int animate(t_game *game)
{
    static int delay = 0;

    delay++;
    if (delay >= 15)
    {
        delay = 0;
        game->frame = (game->frame + 1) % 6;
        
        if (game->is_running)
        {
            game->run_counter++;
            if (game->run_counter >= 15)
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
 * @brief Renders a complete frame
 */
int render_frame(t_game *game)
{
    int i, j;

    // Clear buffer
    ft_memset(game->buffer.addr, 0,
        game->map_width * game->tile_size * game->map_height * 
        game->tile_size * (game->buffer.bpp / 8));

    // Draw floor and walls
    for (i = 0; i < game->map_height; i++)
    {
        for (j = 0; j < game->map_width; j++)
        {
            int x = j * game->tile_size;
            int y = i * game->tile_size;

            // Draw floor
            put_img_to_img(&game->buffer, 
                          game->floor[game->floor_types[i][j]], x, y);

            // Draw walls
            if (game->map[i][j] == '1')
                put_img_to_img(&game->buffer, game->wall, x, y);
            // Draw exit
            else if (game->map[i][j] == 'E')
                put_img_to_img(&game->buffer, game->exit, x, y);
            // Draw player
            else if (game->map[i][j] == 'P')
            {
                void *sprite = game->facing_right ? 
                    (game->is_running ? game->player_run[game->frame] : 
                                      game->player_idle[game->frame]) :
                    (game->is_running ? game->player_run_left[game->frame] : 
                                      game->player_idle_left[game->frame]);
                put_img_to_img(&game->buffer, sprite, x, y);
            }
            // Draw collectibles
            else if (game->map[i][j] == 'C')
                put_img_to_img(&game->buffer, 
                              game->collect[game->frame % 8], x, y);
        }
    }

    // Put buffer to window
    mlx_put_image_to_window(game->mlx, game->win, game->buffer.img_ptr, 0, 0);

    // Draw move counter
    char moves_str[32];
    ft_snprintf(moves_str, sizeof(moves_str), "Moves: %d", game->moves_count);
    mlx_string_put(game->mlx, game->win, 10, 20, 0xFFFFFF, moves_str);

    return (0);
}

/**
 * @brief Handles window expose events
 */
int expose_hook(t_game *game)
{
    return (render_frame(game));
}
