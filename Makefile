NAME = so_long
CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I./includes -I/usr/include -Iincludes
LIBS = -L/usr/local/lib -lmlx -lX11 -lXext -lXrender -lm  

SRCS = srcs/main.c \
       srcs/init.c \
       srcs/render.c \
       srcs/player.c \
       srcs/assets.c \
       srcs/map.c \
       srcs/map_validation.c \
       get_next_line/get_next_line_bonus.c \
       get_next_line/get_next_line_utils_bonus.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(NAME)

%.o: %.c includes/so_long.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

docs: clean_docs
	/sgoinfre/mkurkar/doxygen/doxygen-1.13.2/doxygen-1.13.2/bin/doxygen Doxyfile
	@echo "Documentation generated in docs/html/"
	@echo "Open docs/html/index.html in your browser"

clean_docs:
	rm -rf docs/

.PHONY: all clean fclean re docs clean_docs
