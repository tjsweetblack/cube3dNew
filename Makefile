NAME = cub3d
CC = gcc
CFLAGS = -Wall -Wextra -Werror -O2 -lm
MLX_FLAGS = -lmlx4 -lX11 -lXext
SRCS_DIR = src
SRCS = $(wildcard $(SRCS_DIR)/**/*.c)
OBJS = $(SRCS:src/%.c=%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(MLX_FLAGS) -o $(NAME)

%.o: src/%.c include/cube3d.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all