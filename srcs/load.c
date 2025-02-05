/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkurkar <mkurkar@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:09:38 by mkurkar           #+#    #+#             */
/*   Updated: 2025/02/04 17:49:23 by mkurkar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

/**
 * @brief Loads a sequence of sprite frames from files
 * @param mlx MLX instance pointer
 * @param arr Array to store the loaded sprites
 * @param base_path Base path to the sprite files
 * @param frames Number of frames to load
 *
 * @details
 * Loads a sequence of XPM files named f0.xpm through f{frames-1}.xpm
 * from the specified directory. Each sprite is scaled to 32x32 pixels.
 */
void	load_sprite_array(t_game *game, void **arr, char *base_path, int frames)
{
	char	path[256];
	void	*orig_img;
	void	*scaled_img;
	int		params[3];
	int		i;

	i = 0;
	while (i < frames)
	{
		ft_snprintf(path, sizeof(path), "%s/f%d.xpm", base_path, i);
		orig_img = mlx_xpm_file_to_image(game->mlx, path, &params[0] \
					, &params[1]);
		if (!orig_img)
			handle_error(game, "Failed to load sprite");
		scaled_img = scale_image(game->mlx, orig_img, params[0], params[1]);
		mlx_destroy_image(game->mlx, orig_img);
		if (!scaled_img)
			handle_error(game, "Failed to scale image");
		arr[i++] = scaled_img;
	}
}

static void	load_single_texture(t_game *game, void **dest, char *path)
{
	void	*orig_img;
	char	data[400];

	orig_img = mlx_xpm_file_to_image(game->mlx, path, &game->img_width,
			&game->img_height);
	if (!orig_img)
	{
		ft_snprintf(data, sizeof(data), "Failed to load the texture: %s", path);
		handle_error(game, data);
	}
	*dest = scale_image(game->mlx, orig_img, game->img_width, game->img_height);
	mlx_destroy_image(game->mlx, orig_img);
	if (!*dest)
	{
		ft_snprintf(data, sizeof(data), \
			"Failed to scale the texture: %s", path);
		handle_error(game, data);
	}
}

/**
 * @brief Loads all game assets from files
 * @param game Pointer to game structure
 *
 * @details
 * Loads and initializes:
 * - Floor tiles (10 variants)
 * - Player animations (idle and running)
 * - Collectible items
 * - Wall textures
 * - Exit texture
 * All textures are scaled to match tile_size (32x32)
 */
void	load_assets(t_game *game)
{
	game->tile_size = 32;
	game->img_width = game->tile_size;
	game->img_height = game->tile_size;
	ft_printf("Loading assets...\n");
	load_sprite_array(game, game->floor, "./final_assets/floor", 10);
	load_sprite_array(game, game->player_idle, "./final_assets/idle_knigh",
		6);
	load_sprite_array(game, game->player_run, "./final_assets/run_knight",
		6);
	load_sprite_array(game, game->collect, "./final_assets/chest", 8);
	load_single_texture(game, &game->wall, "./final_assets/wall/f1.xpm");
	load_single_texture(game, &game->exit, "./final_assets/exit.xpm");
	init_floor_types(game);
	ft_printf("All assets loaded successfully!\n");
}
