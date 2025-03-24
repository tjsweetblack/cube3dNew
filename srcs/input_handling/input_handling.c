#include "cube3d.h"

#define MOVE_SPEED 0.1
#define ROT_SPEED 0.05

int handle_keypress(int keycode, t_game *game) {
    if (keycode == 53) // ESC key
        exit_game(game);
    if (keycode == 13) // W key
        move_player(game, MOVE_SPEED, 0);
    if (keycode == 1) // S key
        move_player(game, -MOVE_SPEED, 0);
    if (keycode == 0) // A key
        move_player(game, 0, -MOVE_SPEED);
    if (keycode == 2) // D key
        move_player(game, 0, MOVE_SPEED);
    if (keycode == 123) // Left arrow key
        rotate_player(game, ROT_SPEED);
    if (keycode == 124) // Right arrow key
        rotate_player(game, -ROT_SPEED);
    return (0);
}

int handle_keyrelease(int keycode, t_game *game) {
    (void)keycode;
    (void)game;
    // You can add specific actions on key release if needed
    return (0);
}

void move_player(t_game *game, double move_x, double move_y) {
    double new_x = game->player.x + move_x * game->player.dir_x + move_y * game->player.plane_x;
    double new_y = game->player.y + move_x * game->player.dir_y + move_y * game->player.plane_y;

    if (!check_wall_collision(game, new_x, new_y)) {
        game->player.x = new_x;
        game->player.y = new_y;
    }
}

void rotate_player(t_game *game, double angle) {
    // Rotate direction vector
    double old_dir_x = game->player.dir_x;
    game->player.dir_x = game->player.dir_x * cos(angle) - game->player.dir_y * sin(angle);
    game->player.dir_y = old_dir_x * sin(angle) + game->player.dir_y * cos(angle);

    // Rotate camera plane
    double old_plane_x = game->player.plane_x;
    game->player.plane_x = game->player.plane_x * cos(angle) - game->player.plane_y * sin(angle);
    game->player.plane_y = old_plane_x * sin(angle) + game->player.plane_y * cos(angle);
}

int check_wall_collision(t_game *game, double new_x, double new_y) {
    int map_x = (int)new_x;
    int map_y = (int)new_y;

    if (map_x < 0 || map_x >= game->map.cols || map_y < 0 || map_y >= game->map.rows)
        return (1); // Out of bounds

    return (game->map.grid[map_y][map_x] == '1');
}