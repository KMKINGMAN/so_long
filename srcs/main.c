/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkurkar <mkurkar@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 18:24:47 by mkurkar           #+#    #+#             */
/*   Updated: 2025/01/29 02:50:34 by mkurkar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	exit_game(t_game *game)
{
	cleanup_game(game);
	exit(0);
}

int	close_window(t_game *game)
{
	exit_game(game);
	return (0);
}

int	main(int argc, char **argv)
{
	t_game	game;
	char	*dot;

	if (argc != 2)
	{
		ft_printf("Error: Usage: %s <map_path.ber>\n", argv[0]);
		return (1);
	}
	dot = ft_strrchr(argv[1], '.');
	if (!dot || strcmp(dot, ".ber") != 0)
	{
		ft_printf("Error: Map file must have .ber extension\n");
		return (1);
	}
	ft_memset(&game, 0, sizeof(t_game));
	init_game(&game, argv[1]);
	ft_printf("Starting game loop...\n");
	mlx_hook(game.win, 17, 0, close_window, &game);
	if (game.mlx && game.win)
	{
		render_frame(&game);
		mlx_loop(game.mlx);
	}
	else
	{
		handle_error(&game, "Failed to initialize game");
	}
	return (0);
}
