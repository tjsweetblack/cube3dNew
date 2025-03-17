// In src/utils/error_handling.c
#include <stdio.h>
#include <stdlib.h>

void ft_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

// In src/utils/memory_management.c
#include "cube3d.h"

void free_game(t_game *game) {
    if (!game)
        return;
    if (game->win_ptr)
        mlx_destroy_window(game->mlx_ptr, game->win_ptr);
    if (game->mlx_ptr) {
        for (int i = 0; i < 4; i++) {
            if (game->textures[i].img_ptr)
                mlx_destroy_image(game->mlx_ptr, game->textures[i].img_ptr);
        }
        mlx_destroy_display(game->mlx_ptr);
        free(game->mlx_ptr);
    }
    if (game->map.grid) {
        for (int i = 0; i < game->map.rows; i++) {
            free(game->map.grid[i]);
        }
        free(game->map.grid);
    }
}