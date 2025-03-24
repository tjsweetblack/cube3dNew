NAME = cub3d
CFLAGS = -Wall -Werror -Wextra -g -I./includes -I./lib/minilibx-linux -I./lib/libft
CC = cc
SRCS_DIR = ./srcs
LIBFT_DIR = ./lib/libft
MINILBX_DIR = ./lib/minilibx-linux
MINILIB_LIB = libmlx.a
COMMON_SRC = $(wildcard $(SRCS_DIR)/**/*.c) $(SRCS_DIR)/main.c # Corrected line
OBJ = $(COMMON_SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(MAKE) -C $(MINILBX_DIR)
	$(MAKE) -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $(OBJ) -L$(MINILBX_DIR) -lmlx -lX11 -lXext -lm -L$(LIBFT_DIR) -lft -o $(NAME)
	@echo "Program is ready!!"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) clean -C $(MINILBX_DIR)
	$(MAKE) clean -C $(LIBFT_DIR) #corrected minilibx to libft
	@rm -f $(OBJ)
	@echo "Object files have been cleaned successfully"

fclean: clean
	@rm -f $(NAME)
	@echo "Objects and executables removed"

re: fclean all

#NAME = cub3d
##CC = gcc
##CFLAGS = -Wall -Wextra -Werror -O2 -lm
#MLX_FLAGS = -lmlx -lX11 -lXext
#SRCS_DIR = srcs
#SRCS = $(wildcard $(SRCS_DIR)/**/*.c)
#OBJS = $(SRCS:src/%.c=%.o)

#all: $(NAME)

#$(NAME): $(OBJS)
#	$(CC) $(CFLAGS) $(OBJS) $(MLX_FLAGS) -o $(NAME)

#%.o: src/%.c includes/cube3d.h
#	$(CC) $(CFLAGS) -c $< -o $@

#clean:
#	rm -rf $(OBJS)

#fclean: clean
#	rm -rf $(NAME)

#re: fclean all*#