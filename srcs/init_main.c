/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkurkar <mkurkar@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:17:27 by mkurkar           #+#    #+#             */
/*   Updated: 2025/02/01 19:21:34 by mkurkar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	init_game_vars(t_game *game)
{
	ft_memset(game, 0, sizeof(t_game));
	game->frame = 0;
	game->is_running = 0;
	game->tile_size = 32;
	game->facing_right = 1;
	game->moves_count = 0;
	game->run_counter = 0;
}

void	init_game_window(t_game *game)
{
	int	win_width;
	int	win_height;

	win_width = game->map_width * game->tile_size;
	win_height = game->map_height * game->tile_size;
	game->win = mlx_new_window(game->mlx, win_width, win_height, "so_long");
	if (!game->win)
	{
		cleanup_game(game);
		ft_printf("Error: Failed to create window\n");
		exit(1);
	}
}

static void	init_mlx_and_map(t_game *game, char *map_path)
{
	ft_printf("Initializing MLX...\n");
	game->mlx = mlx_init();
	if (!game->mlx)
	{
		cleanup_game(game);
		ft_printf("Error: MLX initialization failed\n");
		exit(1);
	}
	ft_printf("Reading map: %s\n", map_path);
	game->map = read_map(map_path, &game->map_width, &game->map_height);
	if (!game->map || !validate_map(game))
	{
		ft_printf("Error: Invalid map: %s\n", map_path);
		cleanup_game(game);
		exit(1);
	}
}

/**
 * @brief Main game initialization function
 * @param game Pointer to game structure
 * @param map_path Path to the map file
 *
 * @details Initialization sequence:
 * 1. Reset game state
 * 2. Initialize MLX
 * 3. Load map
 * 4. Create window
 * 5. Load assets
 * 6. Initialize player and collectibles
 * 7. Set up event hooks
 */
void	init_game(t_game *game, char *map_path)
{
	init_game_vars(game);
	init_mlx_and_map(game, map_path);
	init_game_window(game);
	init_buffer(game);
	load_assets(game);
	flip_sprites(game);
	init_player_position(game);
	init_collectibles(game);
	mlx_hook(game->win, 2, 1L << 0, key_hook, game);
	mlx_hook(game->win, 12, 0, expose_hook, game);
	mlx_loop_hook(game->mlx, animate, game);
}
