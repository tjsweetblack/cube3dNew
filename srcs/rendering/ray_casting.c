#include "cube3d.h"

void cast_rays(t_game *game) {
    for (int x = 0; x < WINDOW_WIDTH; x++) {
        // Calculate ray direction
        double camera_x = 2 * x / (double)WINDOW_WIDTH - 1; // x-coordinate in camera space
        double ray_dir_x = game->player.dir_x + game->player.plane_x * camera_x;
        double ray_dir_y = game->player.dir_y + game->player.plane_y * camera_x;

        // Current position in map grid
        int map_x = (int)game->player.x;
        int map_y = (int)game->player.y;

        // Length of ray from current position to next x or y-side
        double side_dist_x;
        double side_dist_y;

        // Length of ray in x or y-direction from one x or y-side to next x or y-side
        double delta_dist_x = (ray_dir_x == 0) ? 1e30 : fabs(1 / ray_dir_x);
        double delta_dist_y = (ray_dir_y == 0) ? 1e30 : fabs(1 / ray_dir_y);
        double perp_wall_dist;

        // What direction to step in x or y-direction (either +1 or -1)
        int step_x;
        int step_y;

        int hit = 0; // Was there a wall hit?
        int side;  // Was a NS or EW wall hit?

        // Calculate step and initial sideDist
        if (ray_dir_x < 0) {
            step_x = -1;
            side_dist_x = (game->player.x - map_x) * delta_dist_x;
        } else {
            step_x = 1;
            side_dist_x = (map_x + 1.0 - game->player.x) * delta_dist_x;
        }
        if (ray_dir_y < 0) {
            step_y = -1;
            side_dist_y = (game->player.y - map_y) * delta_dist_y;
        } else {
            step_y = 1;
            side_dist_y = (map_y + 1.0 - game->player.y) * delta_dist_y;
        }

        // Perform DDA
        while (hit == 0) {
            // Jump to next map square, either in x-direction, or in y-direction
            if (side_dist_x < side_dist_y) {
                side_dist_x += delta_dist_x;
                map_x += step_x;
                side = 0;
            } else {
                side_dist_y += delta_dist_y;
                map_y += step_y;
                side = 1;
            }

            // Check if ray has hit a wall
            if (map_x >= 0 && map_x < game->map.cols && map_y >= 0 && map_y < game->map.rows && game->map.grid[map_y][map_x] == '1')
                hit = 1;
        }

        // Calculate distance projected on camera direction (Euclidean distance would give fisheye effect!)
        if (side == 0)
            perp_wall_dist = (side_dist_x - delta_dist_x);
        else
            perp_wall_dist = (side_dist_y - delta_dist_y);

        // Calculate height of line to draw on screen
        int line_height = (int)(WINDOW_HEIGHT / perp_wall_dist);

        // Calculate lowest and highest pixel to fill in current stripe
        int draw_start = WINDOW_HEIGHT / 2 - line_height / 2;
        if (draw_start < 0)
            draw_start = 0;
        int draw_end = WINDOW_HEIGHT / 2 + line_height / 2;
        if (draw_end >= WINDOW_HEIGHT)
            draw_end = WINDOW_HEIGHT - 1;

        // Choose wall color
        int color = 0xAAAAAA; // Default gray
        if (game->map.grid[map_y][map_x] == '1') {
            if (side == 0)
                color = 0xBBBBBB; // Slightly lighter gray for one side
            else
                color = 0x999999; // Slightly darker gray for the other side
        }

        // Draw the vertical line
        draw_vertical_line(game, x, line_height, color);
    }
}