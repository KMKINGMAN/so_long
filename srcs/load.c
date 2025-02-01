/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkurkar <mkurkar@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:09:38 by mkurkar           #+#    #+#             */
/*   Updated: 2025/02/01 19:12:12 by mkurkar          ###   ########.fr       */
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
void	load_sprite_array(void *mlx, void **arr, char *base_path, int frames)
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
		orig_img = mlx_xpm_file_to_image(mlx, path, &params[0], &params[1]);
		if (!orig_img)
		{
			ft_printf("Error: Failed to load %s\n", path);
			exit(1);
		}
		scaled_img = scale_image(mlx, orig_img, params[0], params[1], 32);
		mlx_destroy_image(mlx, orig_img);
		if (!scaled_img)
		{
			ft_printf("Error: Failed to scale image %s\n", path);
			exit(1);
		}
		arr[i++] = scaled_img;
	}
}

static void	load_single_texture(t_game *game, void **dest, char *path)
{
	void	*orig_img;

	orig_img = mlx_xpm_file_to_image(game->mlx, path, &game->img_width,
			&game->img_height);
	if (!orig_img)
	{
		ft_printf("Error: Failed to load texture: %s\n", path);
		exit(1);
	}
	*dest = scale_image(game->mlx, orig_img, game->img_width, game->img_height,
			game->tile_size);
	mlx_destroy_image(game->mlx, orig_img);
	if (!*dest)
	{
		ft_printf("Error: Failed to scale texture: %s\n", path);
		exit(1);
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
	load_sprite_array(game->mlx, game->floor, "./final_assets/floor", 10);
	load_sprite_array(game->mlx, game->player_idle, "./final_assets/idle_knigh",
		6);
	load_sprite_array(game->mlx, game->player_run, "./final_assets/run_knight",
		6);
	load_sprite_array(game->mlx, game->collect, "./final_assets/chest", 8);
	load_single_texture(game, &game->wall, "./final_assets/wall/f1.xpm");
	load_single_texture(game, &game->exit, "./final_assets/exit.xpm");
	init_floor_types(game);
	ft_printf("All assets loaded successfully!\n");
}
