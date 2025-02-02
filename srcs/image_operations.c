/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkurkar <mkurkar@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 17:17:37 by mkurkar           #+#    #+#             */
/*   Updated: 2025/02/02 16:35:52 by mkurkar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file assets.c
 * @brief Asset loading and manipulation functions
 */

#include "so_long.h"

static void	*create_scaled_image(void *mlx, int n_size)
{
	void	*new_img;

	new_img = mlx_new_image(mlx, n_size, n_size);
	return (new_img);
}

/**
 * Processes image scaling by mapping original image data to a new scaled format
 *
 * @param new_data    Pointer to destination
 * array for scaled image data
 * @param orig_data   Pointer to source array
 * containing original image data
 * @param p          Pointer to array containing
 * scaling parameters where p[4] represents height
 * @param width      Width of the original image
 *
 * @details
 * The function scales the image to a fixed size of 32x32 pixels (n_size).
 * For each pixel in the new scaled image, it maps back to corresponding pixel
 * in original image using proportional scaling calculations.
 *
 * @example
 * int original[100 * 100];  // Original 100x100 image
 * int scaled[32 * 32];      // Will contain 32x32 scaled image
 * int params[5] = {0, 0, 0, 0, 100}; // params where p[4] is height
 * process_scaling(scaled, original, params, 100);
 *
 * @note
 * - Output is always scaled to 32x32 pixels
 * - Function includes bounds checking to prevent buffer overflows
 * - Scaling is done using integer arithmetic for performance
 */
static void	process_scaling(int *new_data, int *orig_data, int *p, int width)
{
	int	i;
	int	j;
	int	n_size;

	i = -1;
	n_size = 32;
	while (++i < n_size)
	{
		j = -1;
		while (++j < n_size)
		{
			if (i * width / n_size * width + j * width / n_size >= 0 && i
				* width / n_size * width + j * width / n_size < (width * p[4]))
				new_data[i * n_size + j] = orig_data[i * width / n_size * width
					+ j * width / n_size];
		}
	}
}

/**
 * @brief Scales an MLX image to a fixed size of 32x32 pixels
 *
 * This function takes an original
 * MLX image and creates a new scaled version
 * with dimensions of 32x32 pixels.
 * It handles the memory allocation and pixel
 * data transfer between the images.
 *
 * @param mlx Pointer to the MLX instance
 * @param original Pointer to the original image to be scaled
 * @param width Original image width in pixels
 * @param height Original image height in pixels
 *
 * @return Pointer to the newly created scaled image,
 * or NULL if an error occurred
 *
 * @note The function internally uses create_scaled_image()
 *  and process_scaling()
 * to handle the actual scaling operation
 *
 * @example
 * void *mlx = mlx_init();
 * void *original_img = mlx_xpm_file_to_image(mlx, "sprite.xpm",
 *  &width, &height);
 * void *scaled_img = scale_image(mlx, original_img,
 *  width, height);
 * if (scaled_img)
 *     mlx_put_image_to_window(mlx, window, scaled_img, x, y);
 */
void	*scale_image(void *mlx, void *original, int width, int height)
{
	void	*new_img;
	int		*orig_data;
	int		*new_data;
	int		params[5];

	new_img = create_scaled_image(mlx, 32);
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
	params[4] = height;
	process_scaling(new_data, orig_data, params, width);
	return (new_img);
}

/**
 * @brief Creates a horizontally flipped copy of an image
 * @param game to get MLX instance pointer
 * @param img Source image to flip
 * @return void* Pointer to the new flipped image
 *
 * @details
 * Creates a new image that is a mirror of the input image.
 * Used for creating left-facing sprites from right-facing ones.
 * Maintains transparency information.
 */
void	*flip_image_horizontal(t_game *game, void *img)
{
	int		params[3];
	void	*new_img;
	char	*data[2];

	new_img = mlx_new_image(game->mlx, 32, 32);
	data[0] = mlx_get_data_addr(new_img, &params[0], &params[1], &params[2]);
	data[1] = mlx_get_data_addr(img, &params[0], &params[1], &params[2]);
	if (!data[0] || !data[1])
	{
		mlx_destroy_image(game->mlx, new_img);
		handle_error(game, "Faild to load the image data");
	}
	params[0] = -1;
	while (++params[0] < 32)
	{
		params[2] = -1;
		while (++params[2] < 32)
		{
			*(unsigned int *)(data[0] + params[0] * params[1] + (32 - 1
						- params[2]) * 4) = *(unsigned int *)(data[1]
					+ params[0] * params[1] + params[2] * 4);
		}
	}
	return (new_img);
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
		game->player_idle_left[i] = flip_image_horizontal(game,
				game->player_idle[i]);
		game->player_run_left[i] = flip_image_horizontal(game,
				game->player_run[i]);
		i++;
	}
}
