NAME = so_long
CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I./includes -I/usr/include -Ilibft -Iincludes
LIBS = -L/usr/local/lib -lmlx -L./libft -lft -lX11 -lXext -lXrender -lm  

SRCS = srcs/main.c \
	srcs/init.c \
	srcs/render.c \
	srcs/player.c \
	srcs/assets.c \
	srcs/map.c \
	srcs/map_validation.c \
	srcs/ft_snprintf.c \
	get_next_line/get_next_line_bonus.c \
	get_next_line/get_next_line_utils_bonus.c

OBJS = $(SRCS:.c=.o)
LIBFT = libft/libft.a

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(NAME)

$(LIBFT):
	$(MAKE) -C libft

%.o: %.c includes/so_long.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(MAKE) -C libft clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C libft fclean
	rm -f $(NAME)

re: fclean all

docs: clean_docs
	doxygen Doxyfile
	@echo "Documentation generated in docs/html/"
	@echo "Open docs/html/index.html in your browser"

clean_docs:
	rm -rf docs/

.PHONY: all clean fclean re docs clean_docs
