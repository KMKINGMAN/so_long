/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkurkar <mkurkar@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 17:17:37 by mkurkar           #+#    #+#             */
/*   Updated: 2025/02/01 19:30:48 by mkurkar          ###   ########.fr       */
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
			if (i * width / n_size * width + j * width / n_size >= 0
				&& i * width / n_size * width + j * width / n_size
				< (width * p[4]))
				new_data[i * n_size + j] = orig_data[i * width / n_size
					* width + j * width / n_size];
		}
	}
}

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
