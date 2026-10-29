CC = cc
INCLUDES = -Iinclude -I./lib/libft/include/ -I./src/internal/
CFLAGS = -Wall -Wextra -Werror -g3

SRC_DIR = ./src/
LIB_DIR = ./lib/
LDFLAGS = -L./lib/libft
LDLIBS = -lft

OBJECTS = $(addprefix $(SRC_DIR), \
		  main.o minishell.o terminal.o user_input.o \
		  ) $(addprefix $(SRC_DIR)builtins/, \
		  echo.o exit.o \
		  ) $(addprefix $(SRC_DIR)lexer/, \
		  lexer.o token.o lexer_attr.o lexer_utils.o\
		  )
TESTOBJECTS = $(addprefix $(SRC_DIR).test/, \
			  main.o debug_utils.o test_utils.o \
			  ) $(filter-out $(SRC_DIR)main.o, $(OBJECTS))
NAME = minishell

all: $(NAME)

test: libs $(TESTOBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) $(TESTOBJECTS) $(LDFLAGS) $(LDLIBS) -o test
	./test

$(NAME): libs $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJECTS) $(LDFLAGS) $(LDLIBS) -o $(NAME)

%.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

libs:
	$(MAKE) -C $(LIB_DIR)

fclean:
	$(MAKE) -C $(LIB_DIR) fclean
	rm -rf $(OBJECTS) $(NAME)
