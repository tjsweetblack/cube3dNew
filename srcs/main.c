#include "cube3d.h"

//gcc main.c srcs/parsing/map_parser.c srcs/textures/texture_loader.c srcs/rendering/ray_casting.c srcs/rendering/draw.c srcs/input_handling/input_handling.c srcs/mini_map/mini_map.c srcs/utils/error_handling.c srcs/utils/memory_management.c lib/libft/libft.a lib/minilibx-linux/libmlx.a -o cube3d -lXext -lX11 -lm

int render_frame(t_game *game) {
    // Clear the window
    mlx_clear_window(game->mlx_ptr, game->win_ptr);
    // Draw floor and ceiling
    draw_floor_ceiling(game);
    // Cast rays and draw walls
    cast_rays(game);
    // Draw minimap
    draw_minimap(game);
    // Put the image to the window
    mlx_put_image_to_window(game->mlx_ptr, game->win_ptr, game->img_ptr, 0, 0);
    return (0);
}

int close_window(void *param) {
    t_game *game = (t_game *)param;
    exit_game(game);
    return (0);
}

int main(int argc, char **argv) {
    t_game game;

    /*if (argc != 2) {
        ft_error("Usage: ./cube3d <map_file.cub>");
        return (1);
    }*/

    // Initialize MLX
    game.mlx_ptr = mlx_init();
    if (!game.mlx_ptr) {
        ft_error("Error initializing MLX");
        return (1);
    }

    // Parse the map
    //if (parse_map(&game, argv[1])) {
    //  return (1);
    // }
    (void)argv;
    (void)argc;
    game.map.rows = 5;
    game.map.cols = 5;
    game.map.grid = malloc(sizeof(char *) * game.map.rows);
    if (!game.map.grid)
        return (1);
    game.map.grid[0] = ft_strdup("1111111");
    game.map.grid[1] = ft_strdup("1000101");
    game.map.grid[2] = ft_strdup("1000001");
    game.map.grid[3] = ft_strdup("10000N1");
    game.map.grid[4] = ft_strdup("1111111");
    game.map.starting_direction = 'N'; // Set a starting direction

    // Initialize player position based on the hardcoded map
    game.player.x = 2.5;
    game.player.y = 1.5;
    game.player.dir_x = -1;
    game.player.dir_y = 0;
    game.player.plane_x = 0;
    game.player.plane_y = 0.66;
    // Create window
    game.win_ptr = mlx_new_window(game.mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT, "Cube3D");
    if (!game.win_ptr) {
        ft_error("Error creating window");
        free_game(&game);
        return (1);
    }

    // Create image buffer
    game.img_ptr = mlx_new_image(game.mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT);
    game.data = (int *)mlx_get_data_addr(game.img_ptr, &game.bits_per_pixel, &game.size_line, &game.endian);

    // Set up event hooks
    mlx_hook(game.win_ptr, 2, 1L << 0, handle_keypress, &game);
    mlx_hook(game.win_ptr, 3, 1L << 1, handle_keyrelease, &game);
    mlx_hook(game.win_ptr, 17, 0, (int (*)())close_window, &game);
    // Set up the render loop
    mlx_loop_hook(game.mlx_ptr, render_frame, &game);

    // Start the MLX loop
    mlx_loop(game.mlx_ptr);

    // Clean up (should not be reached if exit_game is used)
    free_game(&game);
    return (0);
}