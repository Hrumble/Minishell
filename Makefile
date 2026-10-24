CC = cc
INCLUDES = -Iinclude -I./lib/libft/include/
CFLAGS = -Wall -Wextra -Werror

SRC_DIR = ./src/
LIB_DIR = ./lib/
LDFLAGS = -L./lib/libft
LDLIBS = -lft -lreadline

OBJECTS = $(addprefix $(SRC_DIR), \
		  main.o minishell.o terminal.o \
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
