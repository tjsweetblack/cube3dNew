#ifndef CUBE3D_H
#define CUBE3D_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <libft.h>
#include <unistd.h>
#include <mlx.h>

// --- Definitions ---
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define TILE_SIZE 64
#define MINIMAP_SCALE 0.2
#define MINIMAP_TILE_SIZE (int)(TILE_SIZE * MINIMAP_SCALE)
#define MINIMAP_PLAYER_SIZE 8
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

// --- Enums ---
typedef enum { NORTH, SOUTH, EAST, WEST } t_direction;

// --- Structures ---
typedef struct s_color {
    int r;
    int g;
    int b;
} t_color;

typedef struct s_texture {
    void *img_ptr;
    int *data;
    int width;
    int height;
    int bits_per_pixel;
    int size_line;
    int endian;
} t_texture;

typedef struct s_player {
    double x;
    double y;
    double dir_x;
    double dir_y;
    double plane_x;
    double plane_y;
} t_player;

typedef struct s_map {
    char **grid;
    int rows;
    int cols;
    char starting_direction;
} t_map;

typedef struct s_game {
    void *mlx_ptr;
    void *win_ptr;
    void *img_ptr;
    void *data;
    int bits_per_pixel; // Add this
    int size_line; // Add this
    int endian; // Add this
    t_player player;
    t_map map;
    t_texture textures[4]; // NO, SO, WE, EA
    t_color floor_color;
    t_color ceiling_color;
} t_game;

// --- Function Prototypes ---

// parsing/map_parser.c
int parse_map(t_game *game, const char *filepath);

// textures/texture_loader.c
int load_texture(t_game *game, t_texture *texture, char *filepath);

// rendering/raycasting.c
void cast_rays(t_game *game);

// rendering/draw.c
void draw_vertical_line(t_game *game, int x, int height, int color);
void draw_floor_ceiling(t_game *game);

// input/input_handling.c
int handle_keypress(int keycode, t_game *game);
int handle_keyrelease(int keycode, t_game *game);
void move_player(t_game *game, double move_x, double move_y);
void rotate_player(t_game *game, double angle);
int check_wall_collision(t_game *game, double new_x, double new_y);

// minimap/minimap.c
void draw_minimap(t_game *game);
void draw_line(t_game *game, int x1, int y1, int x2, int y2, int color);

// utils/error_handling.c
void ft_error(const char *msg);

// utils/memory_management.c
void exit_game(t_game *game);
void free_game(t_game *game);
void ft_freetab(char **tab);

// GETNEXTLINE
char	*get_next_line(int fd);
size_t	ft_strlen_get(char *s);
char	*ft_strjoin_get(char *s1, char *s2);
char	*ft_strchr_get(char *s, int c);

#endif