/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assets.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkurkar <mkurkar@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 17:17:37 by mkurkar           #+#    #+#             */
/*   Updated: 2025/02/01 19:12:33 by mkurkar          ###   ########.fr       */
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
