#include "cube3d.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static int parse_header(t_game *game, int fd);
static int parse_map_grid(t_game *game, const char *filepath); // Corrected declaration
static int is_valid_map_char(char c);
static int is_map_surrounded(t_game *game);
static int find_player_start(t_game *game);

int parse_map(t_game *game, const char *filepath) {
    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        ft_error("Error opening map file");
        return (1);
    }

    // Initialize map structure
    game->map.grid = NULL;
    game->map.rows = 0;
    game->map.cols = 0;
    game->map.starting_direction = '\0';

    if (parse_header(game, fd)) {
        close(fd);
        free_game(game);
        return (1);
    }
    close(fd); // Close after reading header

    if (parse_map_grid(game, filepath)) { // Corrected line: passing filepath
        free_game(game);
        return (1);
    }

    if (!is_map_surrounded(game)) {
        ft_error("Error: Map is not surrounded by walls");
        free_game(game);
        return (1);
    }

    if (!find_player_start(game)) {
        ft_error("Error: Invalid or missing player starting position");
        free_game(game);
        return (1);
    }

    return (0);
}


static int parse_header(t_game *game, int fd) {
    char *line;
    int texture_count = 0;
    int color_count = 0;

    while ((line = get_next_line(fd)) != NULL) {
        if (ft_strncmp(line, "NO ", 3) == 0) {
            if (load_texture(game, &game->textures[NORTH], ft_strtrim(line + 3, " \t\n")))
                return (free(line), 1);
            texture_count++;
        } else if (ft_strncmp(line, "SO ", 3) == 0) {
            if (load_texture(game, &game->textures[SOUTH], ft_strtrim(line + 3, " \t\n")))
                return (free(line), 1);
            texture_count++;
        } else if (ft_strncmp(line, "WE ", 3) == 0) {
            if (load_texture(game, &game->textures[WEST], ft_strtrim(line + 3, " \t\n")))
                return (free(line), 1);
            texture_count++;
        } else if (ft_strncmp(line, "EA ", 3) == 0) {
            if (load_texture(game, &game->textures[EAST], ft_strtrim(line + 3, " \t\n")))
                return (free(line), 1);
            texture_count++;
        } else if (ft_strncmp(line, "F ", 2) == 0) {
            char **colors = ft_split(ft_strtrim(line + 2, " \t\n"), ',');
            if (colors && colors[0] && colors[1] && colors[2]) {
                game->floor_color.r = ft_atoi(colors[0]);
                game->floor_color.g = ft_atoi(colors[1]);
                game->floor_color.b = ft_atoi(colors[2]);
                color_count++;
            } else {
                ft_error("Error: Invalid floor color format");
                ft_freetab(colors);
                return (free(line), 1);
            }
            ft_freetab(colors);
        } else if (ft_strncmp(line, "C ", 2) == 0) {
            char **colors = ft_split(ft_strtrim(line + 2, " \t\n"), ',');
            if (colors && colors[0] && colors[1] && colors[2]) {
                game->ceiling_color.r = ft_atoi(colors[0]);
                game->ceiling_color.g = ft_atoi(colors[1]);
                game->ceiling_color.b = ft_atoi(colors[2]);
                color_count++;
            } else {
                ft_error("Error: Invalid ceiling color format");
                ft_freetab(colors);
                return (free(line), 1);
            }
            ft_freetab(colors);
        } else if (line[0] == '\n' || line[0] == '\0') {
            // Ignore empty lines
        } else if (ft_strchr("01NSEW ", line[0])) {
            // First map line encountered, header parsing done
            free(line);
            break;
        } else {
            ft_error("Error: Invalid header line");
            return (free(line), 1);
        }
        free(line);
    }

    if (texture_count < 4 || color_count < 2) {
        ft_error("Error: Missing texture paths or floor/ceiling colors in header");
        return (1);
    }

    return (0);
}

static int parse_map_grid(t_game *game, const char *filepath) {
    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        ft_error("Error reopening map file");
        return (1);
    }

    char *line;
    int row = 0;
    int max_cols = 0;
    int num_rows_counted = 0;

    // Skip header lines
    while ((line = get_next_line(fd)) != NULL) {
        if (line[0] == '\n' || line[0] == '\0') {
            free(line);
            continue;
        }
        if (ft_strchr("01NSEW ", line[0])) {
            free(line);
            break; // Found the start of the map
        }
        free(line);
    }

    // First pass to count rows and find max columns
    long current_pos = lseek(fd, 0, SEEK_CUR);
    while ((line = get_next_line(fd)) != NULL) {
        if (line == NULL)
            break;
        if (line[0] == '\n' || line[0] == '\0') {
            free(line);
            continue;
        }
        int cols = ft_strlen(line) - (line[ft_strlen(line) - 1] == '\n');
        if (cols > max_cols)
            max_cols = cols;
        num_rows_counted++;
        free(line);
    }
    lseek(fd, current_pos, SEEK_SET);

    game->map.rows = num_rows_counted;

    game->map.grid = malloc(sizeof(char *) * game->map.rows);
    if (!game->map.grid) {
        ft_error("Error: Failed to allocate memory for map grid");
        close(fd);
        return (1);
    }

    row = 0;
    while ((line = get_next_line(fd)) != NULL && row < game->map.rows) {
        if (line == NULL)
            break;
        if (line[0] == '\n' || line[0] == '\0') {
            free(line);
            continue;
        }
        game->map.grid[row] = malloc(sizeof(char) * (max_cols + 1));
        if (!game->map.grid[row]) {
            ft_error("Error: Failed to allocate memory for map row");
            for (int i = 0; i < row; i++)
                free(game->map.grid[i]);
            free(game->map.grid);
            close(fd);
            return (1);
        }
        ft_memset(game->map.grid[row], ' ', max_cols);
        int len = ft_strlen(line) - (line[ft_strlen(line) - 1] == '\n');
        ft_memcpy(game->map.grid[row], line, len);
        game->map.grid[row][max_cols] = '\0';
        row++;
        free(line);
    }
    game->map.cols = max_cols;

    // Validate map characters
    for (int i = 0; i < game->map.rows; i++) {
        if (!game->map.grid[i])
            continue;
        for (int j = 0; j < game->map.cols; j++) {
            if (!is_valid_map_char(game->map.grid[i][j])) {
                printf("Error: Invalid character found at row %d, col %d: '%c' (ASCII: %d)\n", i, j, game->map.grid[i][j], game->map.grid[i][j]);
                ft_error("Error: Invalid character in map");
                close(fd);
                return (1);
            }
        }
    }

    close(fd);
    return (0);
}

static int is_valid_map_char(char c) {
    return (c == '0' || c == '1' || c == 'N' || c == 'S' || c == 'E' || c == 'W' || c == ' ');
}

static int is_map_surrounded(t_game *game) {
    int rows = game->map.rows;
    int cols = game->map.cols;
    int i, j;

    printf("Map dimensions: rows=%d, cols=%d\n", rows, cols);

    // Check top row
    printf("Top row: ");
    for (j = 0; j < cols; j++) {
        printf("%c", game->map.grid[0][j]);
        if (game->map.grid[0][j] != '1') {
            printf("\nError in top row at col %d: '%c' (ASCII: %d)\n", j, game->map.grid[0][j], game->map.grid[0][j]);
            return (0);
        }
    }
    printf("\n");

    // Check bottom row
    printf("Bottom row: ");
    for (j = 0; j < cols; j++) {
        printf("%c", game->map.grid[rows - 1][j]);
        if (game->map.grid[rows - 1][j] != '1') {
            printf("\nError in bottom row at col %d: '%c' (ASCII: %d)\n", j, game->map.grid[rows - 1][j], game->map.grid[rows - 1][j]);
            return (0);
        }
    }
    printf("\n");

    // Check left column
    printf("Left column:\n");
    for (i = 0; i < rows; i++) {
        printf("%c", game->map.grid[i][0]);
        if (game->map.grid[i][0] != '1') {
            printf("\nError in left column at row %d: '%c' (ASCII: %d)\n", i, game->map.grid[i][0], game->map.grid[i][0]);
            return (0);
        }
    }
    printf("\n");

    // Check right column
    printf("Right column:\n");
    for (i = 0; i < rows; i++) {
        printf("%c", game->map.grid[i][cols - 1]);
        if (game->map.grid[i][cols - 1] != '1') {
            printf("\nError in right column at row %d: '%c' (ASCII: %d)\n", i, game->map.grid[i][cols - 1], game->map.grid[i][cols - 1]);
            return (0);
        }
    }
    printf("\n");

    // Check spaces are surrounded by walls
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (game->map.grid[i][j] == ' ') {
                if ((i > 0 && game->map.grid[i - 1][j] != '1') ||
                    (i < rows - 1 && game->map.grid[i + 1][j] != '1') ||
                    (j > 0 && game->map.grid[i][j - 1] != '1') ||
                    (j < cols - 1 && game->map.grid[i][j + 1] != '1')) {
                    printf("Error: Space at row %d, col %d is not surrounded by '1'\n", i, j);
                    return (0);
                }
            }
        }
    }

    return (1);
}


static int find_player_start(t_game *game) {
    int rows = game->map.rows;
    int cols = game->map.cols;
    int player_count = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (game->map.grid[i][j] == 'N' || game->map.grid[i][j] == 'S' ||
                game->map.grid[i][j] == 'E' || game->map.grid[i][j] == 'W') {
                game->player.x = j + 0.5;
                game->player.y = i + 0.5;
                game->map.starting_direction = game->map.grid[i][j];
                game->map.grid[i][j] = '0'; // Replace starting position with floor
                player_count++;
            }
        }
    }

    if (player_count != 1)
        return (0);

    // Initialize player direction and plane based on starting direction
    if (game->map.starting_direction == 'N') {
        game->player.dir_x = -1;
        game->player.dir_y = 0;
        game->player.plane_x = 0;
        game->player.plane_y = 0.66;
    } else if (game->map.starting_direction == 'S') {
        game->player.dir_x = 1;
        game->player.dir_y = 0;
        game->player.plane_x = 0;
        game->player.plane_y = -0.66;
    } else if (game->map.starting_direction == 'E') {
        game->player.dir_x = 0;
        game->player.dir_y = 1;
        game->player.plane_x = 0.66;
        game->player.plane_y = 0;
    } else if (game->map.starting_direction == 'W') {
        game->player.dir_x = 0;
        game->player.dir_y = -1;
        game->player.plane_x = -0.66;
        game->player.plane_y = 0;
    }

    return (1);
}