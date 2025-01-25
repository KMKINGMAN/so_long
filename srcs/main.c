#include "so_long.h"

static void exit_game(t_game *game)
{
	cleanup_game(game);
	exit(0);
}

int close_window(t_game *game)
{
	exit_game(game);
	return (0);
}

int main(int argc, char **argv)
{
	t_game game;

	if (argc != 2)
	{
		ft_printf("Error: Usage: %s <map_path.ber>\n", argv[0]);
		return (1);
	}

	// Check if file ends with .ber
	char *dot = ft_strrchr(argv[1], '.');
	if (!dot || strcmp(dot, ".ber") != 0)
	{
		ft_printf("Error: Map file must have .ber extension\n");
		return (1);
	}

	ft_memset(&game, 0, sizeof(t_game)); // Initialize to zero

	init_game(&game, argv[1]);
	ft_printf("Starting game loop...\n");

	// Add window close hook
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
