/**
 * @file assets.c
 * @brief Asset loading and manipulation functions
 */

#include "../includes/so_long.h"

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
void *scale_image(void *mlx, void *original, int width, int height, int new_size)
{
	void *new_img;
	int *orig_data;
	int *new_data;
	int bpp;
	int sl;
	int end;

	// Create new image with standard format instead of alpha
	new_img = mlx_new_image(mlx, new_size, new_size);
	if (!new_img)
		return NULL;

	// Get data addresses safely
	orig_data = (int *)mlx_get_data_addr(original, &bpp, &sl, &end);
	if (!orig_data)
	{
		mlx_destroy_image(mlx, new_img);
		return NULL;
	}

	new_data = (int *)mlx_get_data_addr(new_img, &bpp, &sl, &end);
	if (!new_data)
	{
		mlx_destroy_image(mlx, new_img);
		return NULL;
	}

	// Scale image
	for (int y = 0; y < new_size; y++)
	{
		for (int x = 0; x < new_size; x++)
		{
			int src_x = x * width / new_size;
			int src_y = y * height / new_size;
			int src_index = src_y * width + src_x;
			int dst_index = y * new_size + x;

			if (src_index >= 0 && src_index < (width * height) &&
				dst_index >= 0 && dst_index < (new_size * new_size))
			{
				new_data[dst_index] = orig_data[src_index];
			}
		}
	}
	return new_img;
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
void *flip_image_horizontal(void *mlx, void *img)
{
	int width = 32;
	int height = 32;
	int bpp, line_len, endian;
	void *new_img = mlx_new_image(mlx, width, height);
	char *new_data = mlx_get_data_addr(new_img, &bpp, &line_len, &endian);
	char *data = mlx_get_data_addr(img, &bpp, &line_len, &endian);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int src_pos = y * line_len + x * (bpp / 8);
			int dst_pos = y * line_len + (width - 1 - x) * (bpp / 8);
			*(unsigned int *)(new_data + dst_pos) = *(unsigned int *)(data + src_pos);
		}
	}
	return new_img;
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
void load_sprite_array(void *mlx, void **arr, char *base_path, int frames)
{
	char path[256];
	void *orig_img;
	void *scaled_img;
	int width;
	int height;

	for (int i = 0; i < frames; i++)
	{
		ft_snprintf(path, sizeof(path), "%s/f%d.xpm", base_path, i);
		orig_img = mlx_xpm_file_to_image(mlx, path, &width, &height);
		if (!orig_img)
		{
			ft_printf("Error: Failed to load %s\n", path);
			exit(1);
		}

		scaled_img = scale_image(mlx, orig_img, width, height, 32);
		mlx_destroy_image(mlx, orig_img);

		if (!scaled_img)
		{
			ft_printf("Error: Failed to scale image %s\n", path);
			exit(1);
		}
		arr[i] = scaled_img;
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
void flip_sprites(t_game *game)
{
	for (int i = 0; i < 6; i++)
	{
		game->player_idle_left[i] = flip_image_horizontal(game->mlx, game->player_idle[i]);
		game->player_run_left[i] = flip_image_horizontal(game->mlx, game->player_run[i]);
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
void load_assets(t_game *game)
{
	game->tile_size = 32;
	game->img_width = game->tile_size;
	game->img_height = game->tile_size;

	ft_printf("Loading assets...\n");

	// Load all sprites with error handling
	load_sprite_array(game->mlx, game->floor, "./final_assets/floor", 10);
	load_sprite_array(game->mlx, game->player_idle, "./final_assets/idle_knigh", 6);
	load_sprite_array(game->mlx, game->player_run, "./final_assets/run_knight", 6);
	load_sprite_array(game->mlx, game->collect, "./final_assets/chest", 8);

	// Load wall texture
	void *orig_img = mlx_xpm_file_to_image(game->mlx, "./final_assets/wall/f1.xpm",
										   &game->img_width, &game->img_height);
	if (!orig_img)
	{
		ft_printf("Error: Failed to load wall texture\n");
		exit(1);
	}
	game->wall = scale_image(game->mlx, orig_img, game->img_width, game->img_height, game->tile_size);
	mlx_destroy_image(game->mlx, orig_img);

	// Load exit texture
	orig_img = mlx_xpm_file_to_image(game->mlx, "./final_assets/exit.xpm",
									 &game->img_width, &game->img_height);
	if (!orig_img)
	{
		ft_printf("Error: Failed to load exit texture\n");
		exit(1);
	}
	game->exit = scale_image(game->mlx, orig_img, game->img_width, game->img_height, game->tile_size);
	mlx_destroy_image(game->mlx, orig_img);

	if (!game->wall || !game->exit)
	{
		ft_printf("Error: Failed to scale textures\n");
		exit(1);
	}

	init_floor_types(game);
	ft_printf("All assets loaded successfully!\n");
}
