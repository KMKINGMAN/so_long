#include "so_long.h"

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

	init_game(&game, argv[1]);
	ft_printf("Starting game loop...\n");
	render_frame(&game);
	mlx_loop(game.mlx);
	return (0);
}
