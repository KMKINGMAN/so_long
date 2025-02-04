# Compiler and flags
NAME		= so_long
CC		= cc
CFLAGS		= -Wall -Wextra -Werror
INCLUDES	= -I./includes -I/usr/include -Ilibft -Iincludes
LIBS		= -L/usr/local/lib -lmlx -L./libft -lft -lX11 -lXext -lXrender -lm

# Directories
SRC_DIR		= srcs
GNL_DIR		= get_next_line
OBJ_DIR		= obj

# Source files
SRC_FILES	= main.c init.c render.c player.c image_operations.c map.c \
		  map_validation.c ft_snprintf.c utils.c clean_images.c \
		  render_frame.c map_validations_utils.c load.c init_main.c

GNL_FILES	= get_next_line_bonus.c get_next_line_utils_bonus.c

# Convert source files to full paths
SRCS		= $(addprefix $(SRC_DIR)/, $(SRC_FILES)) \
		  $(addprefix $(GNL_DIR)/, $(GNL_FILES))

# Generate object files in obj directory
OBJS		= $(SRCS:%.c=$(OBJ_DIR)/%.o)

# Library
LIBFT		= libft/libft.a

# Colors for output
GREEN		= \033[0;32m
RESET		= \033[0m

# Main targets
all: create_dirs $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@echo "$(GREEN)Linking $(NAME)...$(RESET)"
	@$(CC) $(OBJS) $(LIBS) -o $(NAME)
	@echo "$(GREEN)Build complete!$(RESET)"

$(LIBFT):
	@echo "$(GREEN)Building libft...$(RESET)"
	@$(MAKE) -C libft

# Create necessary directories
create_dirs:
	@mkdir -p $(OBJ_DIR)/$(SRC_DIR) $(OBJ_DIR)/$(GNL_DIR)

# Pattern rule for object files
$(OBJ_DIR)/%.o: %.c includes/so_long.h
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean targets
clean:
	@echo "$(GREEN)Cleaning object files...$(RESET)"
	@$(MAKE) -C libft clean
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "$(GREEN)Cleaning executables...$(RESET)"
	@$(MAKE) -C libft fclean
	@rm -f $(NAME)

re: fclean all

# Documentation targets
docs: clean_docs
	@doxygen Doxyfile
	@echo "$(GREEN)Documentation generated in docs/html/$(RESET)"
	@echo "$(GREEN)Open docs/html/index.html in your browser$(RESET)"

clean_docs:
	@rm -rf docs/

.PHONY: all clean fclean re docs clean_docs create_dirs
