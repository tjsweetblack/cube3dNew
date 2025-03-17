#include "cube3d.h"

#define MOVE_SPEED 0.1
#define ROTATION_SPEED 0.05

int check_wall_collision(t_game *game, double new_x, double new_y) {
    int map_grid_x = (int)new_x;
    int map_grid_y = (int)new_y;
    if (map_grid_y >= 0 && map_grid_y < game->map.rows && map_grid_x >= 0 && map_grid_x < game->map.cols && game->map.grid[map_grid_y][map_grid_x] == '1') {
        return 1; // Collision
    }
    return 0; // No collision
}

void move_player(t_game *game, double move_x, double move_y) {
    double new_x = game->player.x + move_x;
    double new_y = game->player.y + move_y;

    if (!check_wall_collision(game, new_x, game->player.y)) {
        game->player.x = new_x;
    }
    if (!check_wall_collision(game, game->player.x, new_y)) {
        game->player.y = new_y;
    }
}

void rotate_player(t_game *game, double angle) {
    double old_dir_x = game->player.dir_x;
    game->player.dir_x = game->player.dir_x * cos(angle) - game->player.dir_y * sin(angle);
    game->player.dir_y = old_dir_x * sin(angle) + game->player.dir_y * cos(angle);
    double old_plane_x = game->player.plane_x;
    game->player.plane_x = game->player.plane_x * cos(angle) - game->player.plane_y * sin(angle);
    game->player.plane_y = old_plane_x * sin(angle) + game->player.plane_y * cos(angle);
}

int handle_keypress(int keycode, t_game *game) {
    if (keycode == 65307) // Escape
        exit(0);
    if (keycode == 119) // W
        move_player(game, game->player.dir_x * MOVE_SPEED, game->player.dir_y * MOVE_SPEED);
    if (keycode == 115) // S
        move_player(game, -game->player.dir_x * MOVE_SPEED, -game->player.dir_y * MOVE_SPEED);
    if (keycode == 97) // A
        move_player(game, game->player.dir_y * MOVE_SPEED, -game->player.dir_x * MOVE_SPEED);
    if (keycode == 100) // D
        move_player(game, -game->player.dir_y * MOVE_SPEED, game->player.dir_x * MOVE_SPEED);
    if (keycode == 65363) // Right arrow
        rotate_player(game, -ROTATION_SPEED);
    if (keycode == 65361) // Left arrow
        rotate_player(game, ROTATION_SPEED);
    return 0;
}

int handle_keyrelease(int keycode, t_game *game) {
    return 0;
}