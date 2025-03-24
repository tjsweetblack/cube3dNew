#include "cube3d.h"
#include <stdio.h>
#include <stdlib.h>

int load_texture(t_game *game, t_texture *texture, char *filepath) {
    texture->img_ptr = mlx_xpm_file_to_image(game->mlx_ptr, filepath, &texture->width, &texture->height);
    if (!texture->img_ptr) {
        perror("Error loading texture");
        return (1);
    }
    texture->data = (int *)mlx_get_data_addr(texture->img_ptr, &texture->bits_per_pixel, &texture->size_line, &texture->endian);
    return (0);
}