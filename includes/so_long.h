/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkurkar <mkurkar@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:32:23 by mkurkar           #+#    #+#             */
/*   Updated: 2025/02/02 16:23:33 by mkurkar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# define SO_LONG_H

# include "ft_printf.h"
# include "get_next_line_bonus.h"
# include "libft.h"
# include <X11/keysym.h>
# include <fcntl.h>
# include <mlx.h>
# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
# include <unistd.h>

typedef struct s_win
{
	void			*mlx_ptr;
	void			*win_ptr;
	int				height;
	int				width;

}					t_win;

typedef struct s_img
{
	t_win			win;
	void			*img_ptr;
	char			*addr;
	int				h;
	int				w;
	int				bpp;
	int				endian;
	int				line_len;
}					t_img;

typedef struct s_pos
{
	int				row;
	int				col;
	int				dest_x;
	int				dest_y;
}					t_pos;

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
	void			*mlx;
	void			*win;
	char			**map;
	void			*floor[10];
	void			*wall;
	void			*player_idle[6];
	void			*player_run[6];
	void			*collect[8];
	void			*enemy[6];
	void			*exit;
	int				map_width;
	int				map_height;
	int				player_x;
	int				player_y;
	int				exit_x;
	int				exit_y;
	int				frame;
	int				collectibles_count;
	int				img_width;
	int				img_height;
	int				is_running;
	int				can_exit;
	int				moves_count;
	unsigned char	**floor_types;
	int				tile_size;
	int				facing_right;
	t_img			buffer;
	int				run_counter;
	void			*player_idle_left[6];
	void			*player_run_left[6];
}					t_game;

typedef struct s_counts
{
	int				player;
	int				exit;
	int				collect;
}					t_counts;

// init.c
void				init_game(t_game *game, char *map_path);
void				init_player_position(t_game *game);
void				init_collectibles(t_game *game);
void				init_floor_types(t_game *game);

// assets.c
void				load_assets(t_game *game);
void				load_sprite_array(t_game *game, void **arr, char *base_path,
						int frames);
void				*scale_image(void *mlx, void *original, int width,
						int height);

// map.c
char				**read_map(char *file, int *width, int *height);

// player.c
void				move_player(t_game *game, int dx, int dy);
int					key_hook(int keycode, t_game *game);

// render.c
int					render_frame(t_game *game);
int					animate(t_game *game);
int					expose_hook(t_game *game);

void				put_img_to_img(t_img *dest, void *sprite_ptr, int dest_x,
						int dest_y);
void				init_buffer(t_game *game);

void				flip_sprites(t_game *game);

// map_validation.c
int					validate_map(t_game *game);
void				free_map(char **map, int height);

int					ft_snprintf(char *str, size_t size, const char *format,
						...);

void				cleanup_game(t_game *game);
void				handle_error(t_game *game, const char *message);

int					close_window(t_game *game);
void				cleanup_scaled_images(t_game *game);
int					check_map_borders(t_game *game);
int					check_elements(t_game *game, int i, int j, t_counts *count);
int					check_map_content(t_game *game, t_counts *count);
int					validate_counts(t_game *game, t_counts count);
void				init_game_window(t_game *game);
#endif
