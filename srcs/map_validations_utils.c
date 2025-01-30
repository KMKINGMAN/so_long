/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validations_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkurkar <mkurkar@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 17:02:20 by mkurkar           #+#    #+#             */
/*   Updated: 2025/01/30 17:06:58 by mkurkar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

/**
 * @brief Simple map validation check for basic requirements
 * @param game Pointer to game structure
 * @return int 1 if valid, 0 if invalid
 */
int	check_map_borders(t_game *game)
{
	int	j;

	if (game->map_height < 3 || game->map_width < 3)
		return (ft_printf("Error: Map too small\n") && 0);
	j = -1;
	while (++j < game->map_width)
		if (game->map[0][j] != '1' || game->map[game->map_height - 1][j] != '1')
			return (ft_printf("Error: Map not surrounded by walls\n") && 0);
	return (1);
}

int	check_elements(t_game *game, int i, int j, t_counts *count)
{
	char	c;

	c = game->map[i][j];
	if (c == 'P')
	{
		count->player++;
		game->player_x = j;
		game->player_y = i;
	}
	else if (c == 'E')
	{
		count->exit++;
		game->exit_x = j;
		game->exit_y = i;
	}
	else if (c == 'C')
		count->collect++;
	else if (c != '0' && c != '1')
		return (ft_printf("Error: Invalid character in map\n") && 0);
	return (1);
}

int	check_map_content(t_game *game, t_counts *count)
{
	int	i;
	int	j;

	i = -1;
	while (++i < game->map_height)
	{
		if ((int)ft_strlen(game->map[i]) != game->map_width)
			return (ft_printf("Error: Map not rectangular\n") && 0);
		if (game->map[i][0] != '1' || game->map[i][game->map_width - 1] != '1')
			return (ft_printf("Error: Map not surrounded by walls\n") && 0);
		j = -1;
		while (++j < game->map_width)
			if (!check_elements(game, i, j, count))
				return (0);
	}
	return (1);
}

int	validate_counts(t_game *game, t_counts count)
{
	if (count.player != 1)
		return (ft_printf("Error: Must have exactly one player\n") && 0);
	if (count.exit != 1)
		return (ft_printf("Error: Must have exactly one exit\n") && 0);
	if (count.collect < 1)
		return (ft_printf("Error: Must have at least one collectible\n") && 0);
	game->collectibles_count = count.collect;
	return (1);
}
