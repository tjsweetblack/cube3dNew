#include "cube3d.h"

void cast_rays(t_game *game) {
    for (int x = 0; x < WINDOW_WIDTH; x++) {
        double camera_x = 2 * x / (double)WINDOW_WIDTH - 1; // Normalized screen coordinate (-1 to +1)
        double ray_dir_x = game->player.dir_x + game->player.plane_x * camera_x;
        double ray_dir_y = game->player.dir_y + game->player.plane_y * camera_x;

        int map_x = (int)game->player.x;
        int map_y = (int)game->player.y;

        double side_dist_x;
        double side_dist_y;

        double delta_dist_x = (ray_dir_x == 0) ? 1e30 : fabs(1 / ray_dir_x);
        double delta_dist_y = (ray_dir_y == 0) ? 1e30 : fabs(1 / ray_dir_y);
        double perp_wall_dist;

        int step_x;
        int step_y;
        int side; 

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

        // Perform DDA (Digital Differential Analyzer)
        int hit = 0;
        while (hit == 0) {
            // Jump to next map square, either in x- or y-direction
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
            if (map_y >= 0 && map_y < game->map.rows && map_x >= 0 && map_x < game->map.cols && game->map.grid[map_y][map_x] == '1') {
                hit = 1;
            }
        }
        // Calculate distance to the wall (Euclidean distance with fish-eye correction)
        if (side == 0) {
            perp_wall_dist = (side_dist_x - delta_dist_x);
        } else {
            perp_wall_dist = (side_dist_y - delta_dist_y);
        }
        // Calculate height of line to draw on screen
        int line_height = (int)(WINDOW_HEIGHT / perp_wall_dist);
        // Calculate lowest and highest pixel to fill current stripe
        int draw_start = -line_height / 2 + WINDOW_HEIGHT / 2;
        if (draw_start < 0) draw_start = 0;
        int draw_end = line_height / 2 + WINDOW_HEIGHT / 2;
        if (draw_end >= WINDOW_HEIGHT) draw_end = WINDOW_HEIGHT - 1;
        // Determine which texture to use
        int tex_num = 0; // Default texture index
        if (side == 0 && ray_dir_x > 0) tex_num = 0;  // West
        else if (side == 0 && ray_dir_x < 0) tex_num = 1; // East
        else if (side == 1 && ray_dir_y > 0) tex_num = 2; // North
        else if (side == 1 && ray_dir_y < 0) tex_num = 3; // South
        // Calculate exact position of wall hit
        double wall_x;
        if (side == 0) wall_x = game->player.y + perp_wall_dist * ray_dir_y;
        else           wall_x = game->player.x + perp_wall_dist * ray_dir_x;
        wall_x -= floor(wall_x);

        // x coordinate on the texture
        int tex_x = (int)(wall_x * (double)game->textures[tex_num].width);
        if(side == 0 && ray_dir_x > 0) tex_x = game->textures[tex_num].width - tex_x - 1;
        if(side == 1 && ray_dir_y < 0) tex_x = game->textures[tex_num].width - tex_x - 1;
        // Draw the vertical line with the texture
        for (int y = draw_start; y < draw_end; y++) {
            int tex_y = (((y - draw_start) * game->textures[tex_num].height) / line_height);
            if(tex_y < 0) tex_y = 0;
            if(tex_y >= game->textures[tex_num].height) tex_y = game->textures[tex_num].height - 1;
            int color = game->textures[tex_num].data[game->textures[tex_num].width * tex_y + tex_x];
            mlx_pixel_put(game->mlx_ptr, game->win_ptr, x, y, color);
        }
    }
    draw_floor_ceiling(game);
    draw_minimap(game); // Draw minimap after rendering the scene
}