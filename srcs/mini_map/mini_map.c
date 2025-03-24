#include "cube3d.h"

void draw_minimap(t_game *game) {
    int map_width = game->map.cols;
    int map_height = game->map.rows;
    double player_x = game->player.x;
    double player_y = game->player.y;

    for (int y = 0; y < map_height; y++) {
        for (int x = 0; x < map_width; x++) {
            int color;
            if (game->map.grid[y][x] == '1')
                color = 0xFFFFFF; // White for walls
            else if (game->map.grid[y][x] == '0' || game->map.grid[y][x] == game->map.starting_direction)
                color = 0x000000; // Black for floor/empty
            else
                continue; // Skip other characters

            for (int i = 0; i < MINIMAP_TILE_SIZE; i++) {
                for (int j = 0; j < MINIMAP_TILE_SIZE; j++) {
                    int pixel_x = x * MINIMAP_TILE_SIZE + i;
                    int pixel_y = y * MINIMAP_TILE_SIZE + j;
                    if (pixel_x >= 0 && pixel_x < WINDOW_WIDTH && pixel_y >= 0 && pixel_y < WINDOW_HEIGHT)
                        mlx_pixel_put(game->mlx_ptr, game->win_ptr, pixel_x, pixel_y, color);
                }
            }
        }
    }

    // Draw player
    int player_pixel_x = (int)(player_x * MINIMAP_SCALE * TILE_SIZE);
    int player_pixel_y = (int)(player_y * MINIMAP_SCALE * TILE_SIZE);
    int player_size = MINIMAP_PLAYER_SIZE;
    for (int i = -player_size / 2; i < player_size / 2; i++) {
        for (int j = -player_size / 2; j < player_size / 2; j++) {
            int x = player_pixel_x + i;
            int y = player_pixel_y + j;
            if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT)
                mlx_pixel_put(game->mlx_ptr, game->win_ptr, x, y, 0xFF0000); // Red for player
        }
    }

    // Draw player's direction (simplified)
    int direction_x = player_pixel_x + (int)(game->player.dir_x * 10);
    int direction_y = player_pixel_y + (int)(game->player.dir_y * 10);
    draw_line(game, player_pixel_x, player_pixel_y, direction_x, direction_y, 0x00FF00); // Green for direction
}

void draw_line(t_game *game, int x1, int y1, int x2, int y2, int color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        if (x1 >= 0 && x1 < WINDOW_WIDTH && y1 >= 0 && y1 < WINDOW_HEIGHT)
            mlx_pixel_put(game->mlx_ptr, game->win_ptr, x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
}