#include "../includes/so_long.h"

/**
 * @file player.c
 * @brief Player movement and interaction handling
 */

/**
 * @brief Handles player movement and collision
 * 
 * @param game Pointer to game structure
 * @param dx   X-axis movement (-1, 0, 1)
 * @param dy   Y-axis movement (-1, 0, 1)
 * 
 * @details
 * - Updates player position if move is valid
 * - Handles collectible pickup
 * - Manages exit condition
 * - Updates player direction and animation state
 */
void move_player(t_game *game, int dx, int dy)
{
	int new_x = game->player_x + dx;
	int new_y = game->player_y + dy;

	// Update direction
	if (dx > 0)
		game->facing_right = 1;
	else if (dx < 0)
		game->facing_right = 0;

	if (game->map[new_y][new_x] == '1')
		return;

	if (game->map[new_y][new_x] == 'C')
	{
		game->collectibles_count--;
		if (game->collectibles_count == 0)
		{
			game->can_exit = 1;
			printf("All collectibles gathered! Go to exit!\n");
		}
	}

	if (game->map[new_y][new_x] == 'E')
	{
		if (game->can_exit)
		{
			printf("You won in %d moves!\n", game->moves_count + 1);
			mlx_destroy_window(game->mlx, game->win);
			exit(0);
		}
		printf("Collect all items first!\n");
		return;
	}

	game->map[game->player_y][game->player_x] = '0';
	game->map[new_y][new_x] = 'P';
	game->player_x = new_x;
	game->player_y = new_y;
	game->is_running = 1;
	game->run_counter = 0;  // Reset run counter when moving
	game->moves_count++;
}

/**
 * @brief Handles keyboard input events
 * @param keycode The key code of the pressed key
 * @param game Pointer to the game structure
 * @return int Always returns 0
 * 
 * @details Controls:
 * - ESC (65307): Exits the game
 * - W/Up (13/119): Move up
 * - S/Down (1/115): Move down
 * - A/Left (0/97): Move left
 * - D/Right (2/100): Move right
 */
int key_hook(int keycode, t_game *game)
{
	if (keycode == 65307)
	{
		mlx_destroy_window(game->mlx, game->win);
		exit(0);
	}
	else if (keycode == 13 || keycode == 119)
		move_player(game, 0, -1);
	else if (keycode == 1 || keycode == 115)
		move_player(game, 0, 1);
	else if (keycode == 0 || keycode == 97)
		move_player(game, -1, 0);
	else if (keycode == 2 || keycode == 100)
		move_player(game, 1, 0);
	return (0);
}
