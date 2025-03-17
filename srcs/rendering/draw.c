#include "cube3d.h"

void draw_vertical_line(t_game *game, int x, int height, int color) {
    int start = (WINDOW_HEIGHT - height) / 2;
    if (start < 0) start = 0;
    int end = start + height;
    if (end >= WINDOW_HEIGHT) end = WINDOW_HEIGHT;
    for (int y = start; y < end; y++) {
        mlx_pixel_put(game->mlx_ptr, game->win_ptr, x, y, color);
    }
}

void draw_floor_ceiling(t_game *game) {
    for (int y = 0; y < WINDOW_HEIGHT / 2; y++) {
        for (int x = 0; x < WINDOW_WIDTH; x++) {
            mlx_pixel_put(game->mlx_ptr, game->win_ptr, x, y, (game->ceiling_color.r << 16) | (game->ceiling_color.g << 8) | game->ceiling_color.b);
        }
    }
    for (int y = WINDOW_HEIGHT / 2; y < WINDOW_HEIGHT; y++) {
        for (int x = 0; x < WINDOW_WIDTH; x++) {
            mlx_pixel_put(game->mlx_ptr, game->win_ptr, x, y, (game->floor_color.r << 16) | (game->floor_color.g << 8) | game->floor_color.b);
        }
    }
}