#include "cube3d.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <map_file.cub>\n", argv[0]);
        return 1;
    }

    t_game game;
    game.mlx_ptr = mlx_init();
    if (!game.mlx_ptr)
        return 1;
    game.win_ptr = mlx_new_window(game.mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT, "Cube3D");
    if (!game.win_ptr)
        return 1;

    // Initialize game state
    ft_memset(&game, 0, sizeof(t_game));

    // Parse the map
    if (parse_map(&game, argv[1])) {
        free_game(&game);
        return 1;
    }

    // Load textures (replace with your actual texture paths)
    if (load_texture(&game, &game.textures[0], "textures/wall_1.xpm") ||
        load_texture(&game, &game.textures[1], "textures/wall_2.xpm") ||
        load_texture(&game, &game.textures[2], "textures/wall_3.xpm") ||
        load_texture(&game, &game.textures[3], "textures/wall_4.xpm")) {
        free_game(&game);
        return 1;
    }

    // Set floor and ceiling colors (example)
    game.floor_color = (t_color){184, 113, 39};
    game.ceiling_color = (t_color){51, 198, 227};

    // Set up event hooks
    mlx_hook(game.win_ptr, 2, 1L << 0, handle_keypress, &game);
    mlx_hook(game.win_ptr, 3, 1L << 1, handle_keyrelease, &game);
    mlx_hook(game.win_ptr, 17, 0, (void *)exit, 0); // Handle window close button

    // Game loop function
    mlx_loop_hook(game.mlx_ptr, (void (*)(void *))cast_rays, &game);

    mlx_loop(game.mlx_ptr);

    free_game(&game);
    return 0;
}