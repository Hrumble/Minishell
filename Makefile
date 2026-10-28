CC = cc
INCLUDES = -Iinclude -I./lib/libft/include/ -I./src/internal/
CFLAGS = -Wall -Wextra -Werror -g3

SRC_DIR = ./src/
LIB_DIR = ./lib/
LDFLAGS = -L./lib/libft
LDLIBS = -lft

OBJECTS = $(addprefix $(SRC_DIR), \
		  main.o minishell.o terminal.o parser.o user_input.o debug_utils.o split_cmd_line.o\
		  ) $(addprefix $(SRC_DIR)builtins/, \
		  echo.o exit.o\
		  )
NAME = minishell

all: $(NAME)

$(NAME): libs $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJECTS) $(LDFLAGS) $(LDLIBS) -o $(NAME)

$(OBJECTS): %.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

libs:
	$(MAKE) -C $(LIB_DIR)

fclean:
	$(MAKE) -C $(LIB_DIR) fclean
	rm -rf $(OBJECTS) $(NAME)
