#include "cube3d.h"

void draw_minimap(t_game *game) {
    int map_width = game->map.cols;
    int map_height = game->map.rows;

    for (int y = 0; y < map_height; y++) {
        for (int x = 0; x < map_width; x++) {
            int pixel_x = x * MINIMAP_TILE_SIZE;
            int pixel_y = y * MINIMAP_TILE_SIZE;
            int color = 0;

            if (game->map.grid[y][x] == '1')
                color = 0xFFFFFF; // White for walls
            else if (game->map.grid[y][x] == '0')
                color = 0x000000; // Black for floor

            for (int i = 0; i < MINIMAP_TILE_SIZE; i++) {
                for (int j = 0; j < MINIMAP_TILE_SIZE; j++) {
                    if (pixel_x + i < WINDOW_WIDTH && pixel_y + j < WINDOW_HEIGHT)
                        mlx_pixel_put(game->mlx_ptr, game->win_ptr, pixel_x + i, pixel_y + j, color);
                }
            }
        }
    }

    // Draw player on minimap
    int player_x_px = (int)(game->player.x * MINIMAP_TILE_SIZE);
    int player_y_px = (int)(game->player.y * MINIMAP_TILE_SIZE);
    int player_color = 0xFF0000; // Red for player

    for (int i = -MINIMAP_PLAYER_SIZE / 2; i < MINIMAP_PLAYER_SIZE / 2; i++) {
        for (int j = -MINIMAP_PLAYER_SIZE / 2; j < MINIMAP_PLAYER_SIZE / 2; j++) {
            if (player_x_px + i >= 0 && player_x_px + i < WINDOW_WIDTH &&
                player_y_px + j >= 0 && player_y_px + j < WINDOW_HEIGHT)
                mlx_pixel_put(game->mlx_ptr, game->win_ptr, player_x_px + i, player_y_px + j, player_color);
        }
    }
}