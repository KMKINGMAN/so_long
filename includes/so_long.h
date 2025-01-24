/**
 * @file so_long.h
 * @brief Main header file for the so_long game
 * @author Your Name
 * @version 1.0
 * 
 * Contains all structure definitions and function prototypes
 * for the so_long 2D game implementation.
 */

#ifndef SO_LONG_H
#define SO_LONG_H

#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "get_next_line_bonus.h"
#include <stdarg.h>
#include "libft.h"
#include "ft_printf.h"

typedef struct s_win
{
	void	*mlx_ptr;
	void	*win_ptr;
	int		height;
	int		width;



	
}		t_win;

typedef struct s_img
{
	t_win	win;
	void	*img_ptr;
	char	*addr;
	int		h;
	int		w;
	int		bpp;
	int		endian;
	int		line_len;
}		t_img;

/**
 * @struct t_game
 * @brief Main game structure containing all game state and resources
 * 
 * @param mlx         MLX instance pointer
 * @param win         Window pointer
 * @param map         2D array representing the game map
 * @param floor       Array of floor tile textures
 * @param wall        Wall texture
 * @param player_idle Array of player idle animation frames
 * @param player_run  Array of player running animation frames
 * @param collect    Array of collectible item textures
 * @param enemy      Array of enemy textures
 * @param exit       Exit texture
 * ...
 */
typedef struct s_game
{
	void *mlx;
	void *win;
	char **map;
	void *floor[10];
	void *wall;
	void *player_idle[6];
	void *player_run[6];
	void *collect[8];
	void *enemy[6];
	void *exit;
	int map_width;
	int map_height;
	int player_x;
	int player_y;
	int exit_x;
	int exit_y;
	int frame;
	int collectibles_count;
	int img_width;
	int img_height;
	int is_running;
	int can_exit;
	int moves_count;
	unsigned char **floor_types; // Changed from char to unsigned char
	int tile_size;
	int facing_right;
	t_img buffer;    // Add this new field
	int run_counter;  // Add this new field
	void *player_idle_left[6];  // Add these new arrays
	void *player_run_left[6];   // for left-facing sprites
} t_game;


// init.c
void init_game(t_game *game, char *map_path); // Modified to accept map path
void init_player_position(t_game *game);
void init_collectibles(t_game *game);
void init_floor_types(t_game *game);

// assets.c
void load_assets(t_game *game);
void load_sprite_array(void *mlx, void **arr, char *base_path, int frames);
void *scale_image(void *mlx, void *original, int width, int height, int new_size);

// map.c
char **read_map(char *file, int *width, int *height);

// player.c
void move_player(t_game *game, int dx, int dy);
int key_hook(int keycode, t_game *game);

// render.c
int render_frame(t_game *game);
int animate(t_game *game);
int expose_hook(t_game *game);

void    put_img_to_img(t_img *dst, void *src_ptr, int x, int y);
void    init_buffer(t_game *game);

void flip_sprites(t_game *game);

// map_validation.c
int validate_map(t_game *game);
void free_map(char **map, int height);

int ft_snprintf(char *str, size_t size, const char *format, ...);

#endif

