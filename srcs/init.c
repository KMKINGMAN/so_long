#include "../includes/so_long.h"

/**
 * @brief Initializes random floor tile variations
 * @param game Pointer to game structure
 *
 * @details
 * Creates a 2D array of floor type indices.
 * Each position gets a random floor variant (0-9)
 * for visual variety in the game.
 */
void init_floor_types(t_game *game)
{
	int i, j;

	game->floor_types = (unsigned char **)malloc(sizeof(unsigned char *) * game->map_height);
	for (i = 0; i < game->map_height; i++)
	{
		game->floor_types[i] = (unsigned char *)malloc(sizeof(unsigned char) * game->map_width);
		for (j = 0; j < game->map_width; j++)
		{
			game->floor_types[i][j] = (unsigned char)(rand() % 10);
		}
	}
}

/**
 * @brief Locates and sets the player's starting position
 * @param game Pointer to game structure
 *
 * @details
 * Scans the map for 'P' character and sets
 * player_x and player_y coordinates accordingly.
 */
void init_player_position(t_game *game)
{
	int i = 0;
	int j = 0;

	while (game->map[i])
	{
		j = 0;
		while (game->map[i][j])
		{
			if (game->map[i][j] == 'P')
			{
				game->player_x = j;
				game->player_y = i;
				return;
			}
			j++;
		}
		i++;
	}
}

/**
 * @brief Initializes collectible items and exit
 * @param game Pointer to game structure
 *
 * @details
 * - Counts total collectibles ('C' in map)
 * - Locates exit position ('E' in map)
 * - Initializes completion tracking
 */
void init_collectibles(t_game *game)
{
	int i = 0;
	int j = 0;
	game->collectibles_count = 0;

	while (game->map[i])
	{
		j = 0;
		while (game->map[i][j])
		{
			if (game->map[i][j] == 'C')
				game->collectibles_count++;
			else if (game->map[i][j] == 'E')
			{
				game->exit_x = j;
				game->exit_y = i;
			}
			j++;
		}
		i++;
	}
	game->can_exit = 0;
}

/**
 * @brief Main game initialization function
 * @param game Pointer to game structure
 * @param map_path Path to the map file
 *
 * @details Initialization sequence:
 * 1. Reset game state
 * 2. Initialize MLX
 * 3. Load map
 * 4. Create window
 * 5. Load assets
 * 6. Initialize player and collectibles
 * 7. Set up event hooks
 */
void init_game(t_game *game, char *map_path)
{
	ft_memset(game, 0, sizeof(t_game));
	game->frame = 0;
	game->is_running = 0;
	game->tile_size = 32;
	game->facing_right = 1;
	game->moves_count = 0;
	game->run_counter = 0; // Add this line after other initializations

	ft_printf("Initializing MLX...\n");
	game->mlx = mlx_init();
	if (!game->mlx)
	{
		ft_printf("Error: MLX initialization failed\n");
		exit(1);
	}

	ft_printf("Reading map: %s\n", map_path);
	game->map = read_map(map_path, &game->map_width, &game->map_height);
	if (!game->map || !validate_map(game))
	{
		ft_printf("Error: Invalid map: %s\n", map_path);
		if (game->map)
			free_map(game->map, game->map_height);
		exit(1);
	}

	int win_width = game->map_width * game->tile_size;
	int win_height = game->map_height * game->tile_size;
	game->win = mlx_new_window(game->mlx, win_width, win_height, "so_long");

	init_buffer(game); // Add this line
	load_assets(game);
	flip_sprites(game); // Add this line after loading assets
	init_player_position(game);
	init_collectibles(game);

	mlx_hook(game->win, 2, 1L << 0, key_hook, game);
	mlx_hook(game->win, 12, 0, expose_hook, game);
	mlx_loop_hook(game->mlx, animate, game);
}
