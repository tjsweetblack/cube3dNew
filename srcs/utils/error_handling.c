#include "cube3d.h"
#include <stdio.h>
#include <stdlib.h>

void ft_error(const char *msg) {
    perror(msg);
}

void exit_game(t_game *game) {
    // Free textures
    for (int i = 0; i < 4; i++) {
        if (game->textures[i].img_ptr)
            mlx_destroy_image(game->mlx_ptr, game->textures[i].img_ptr);
    }

    // Free map grid
    if (game->map.grid) {
        for (int i = 0; i < game->map.rows; i++) {
            if (game->map.grid[i])
                free(game->map.grid[i]);
        }
        free(game->map.grid);
    }

    // Destroy window and MLX instance
    if (game->win_ptr)
        mlx_destroy_window(game->mlx_ptr, game->win_ptr);
    if (game->mlx_ptr) {
        mlx_destroy_display(game->mlx_ptr);
        free(game->mlx_ptr);
    }
    exit(EXIT_SUCCESS);
}