/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assets.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkurkar <mkurkar@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 17:17:37 by mkurkar           #+#    #+#             */
/*   Updated: 2025/01/25 18:25:01 by mkurkar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file assets.c
 * @brief Asset loading and manipulation functions
 */

#include "so_long.h"

/**
 * @brief Scales an image to a new size
 *
 * @param mlx      MLX instance pointer
 * @param original Original image
 * @param width    Original width
 * @param height   Original height
 * @param new_size Target size
 * @return void*   Scaled image pointer or NULL on failure
 *
 * @details
 * Creates a new scaled version of the input image using
 * nearest neighbor interpolation.
 */
void	*scale_image(void *mlx, void *original, int width, int height,
		int n_size)
{
	void	*new_img;
	int		*orig_data;
	int		*new_data;
	int		params[5];

	new_img = mlx_new_image(mlx, n_size, n_size);
	if (!new_img)
		return (NULL);
	orig_data = (int *)mlx_get_data_addr(original, &params[0], &params[1],
			&params[2]);
	if (!orig_data)
	{
		mlx_destroy_image(mlx, new_img);
		return (NULL);
	}
	new_data = (int *)mlx_get_data_addr(new_img, &params[0], &params[1],
			&params[2]);
	if (!new_data)
	{
		mlx_destroy_image(mlx, new_img);
		return (NULL);
	}
	params[3] = -1;
	while (++params[3] < n_size)
	{
		params[4] = -1;
		while (++params[4] < n_size)
			if (params[3] * width / n_size * width + params[4] * width
				/ n_size >= 0 && params[3] * width / n_size * width + params[4]
				* width / n_size < (width * height))
				new_data[params[3] * n_size + params[4]] = orig_data[params[3]
					* width / n_size * width + params[4] * width / n_size];
	}
	return (new_img);
}

/**
 * @brief Creates a horizontally flipped copy of an image
 * @param mlx MLX instance pointer
 * @param img Source image to flip
 * @return void* Pointer to the new flipped image
 *
 * @details
 * Creates a new image that is a mirror of the input image.
 * Used for creating left-facing sprites from right-facing ones.
 * Maintains transparency information.
 */
void	*flip_image_horizontal(void *mlx, void *img)
{
	int		width;
	int		height;
	int		params[3];
	void	*new_img;
	char	*data[2];

	width = 32;
	height = 32;
	new_img = mlx_new_image(mlx, width, height);
	data[0] = mlx_get_data_addr(new_img, &params[0], &params[1], &params[2]);
	data[1] = mlx_get_data_addr(img, &params[0], &params[1], &params[2]);
	params[0] = -1;
	while (++params[0] < height)
	{
		params[2] = -1;
		while (++params[2] < width)
		{
			*(unsigned int *)(data[0] + params[0] * params[1] + (width - 1
						- params[2]) * 4) = *(unsigned int *)(data[1]
					+ params[0] * params[1] + params[2] * 4);
		}
	}
	return (new_img);
}

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
		arr[i] = scaled_img;
		i++;
	}
}

/**
 * @brief Creates left-facing versions of all player sprites
 * @param game Pointer to game structure
 *
 * @details
 * Creates mirrored versions of both idle and running animations
 * for the player character. These are used when the player is
 * moving or facing left.
 */
void	flip_sprites(t_game *game)
{
	int	i;

	i = 0;
	while (i < 6)
	{
		game->player_idle_left[i] = flip_image_horizontal(game->mlx,
				game->player_idle[i]);
		game->player_run_left[i] = flip_image_horizontal(game->mlx,
				game->player_run[i]);
		i++;
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
