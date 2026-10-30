CC = cc
INCLUDES = -Iinclude -I./lib/libft/include/ -I./src/internal/

# REMOVE THE FOLLOWING LINE
INCLUDES += -I./src/.debug/

CFLAGS = -Wall -Wextra -Werror -g3

SRC_DIR = ./src/
LIB_DIR = ./lib/
LDFLAGS = -L./lib/libft
LDLIBS = -lft

OBJECTS = $(addprefix $(SRC_DIR), \
		  main.o minishell.o terminal.o user_input.o expand.o expand_utils.o\
		  ) $(addprefix $(SRC_DIR)builtins/, \
		  echo.o exit.o \
		  ) $(addprefix $(SRC_DIR)lexer/, \
		  lexer.o token.o lexer_attr.o lexer_utils.o\
		  ) $(addprefix $(SRC_DIR)parser/, \
		  parser.o parser_utils.o parser_attr.o parse_ast.o ast_nodes.o ast_free.o \
		  )
DEBUGOBJECTS = $(addprefix $(SRC_DIR).debug/, \
		print_utils.o enum_utils.o \
		)
TESTOBJECTS = $(addprefix $(SRC_DIR).test/, \
			  main.o \
			  ) \
			  $(filter-out $(SRC_DIR)main.o, $(OBJECTS))
NAME = minishell

all: $(NAME)

test: libs $(TESTOBJECTS) $(DEBUGOBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) $(TESTOBJECTS) \
		$(DEBUGOBJECTS) $(LDFLAGS) $(LDLIBS) -o ms_test
	./ms_test

$(NAME): libs $(OBJECTS) $(DEBUGOBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJECTS) \
		$(DEBUGOBJECTS) $(LDFLAGS) $(LDLIBS) -o $(NAME)

%.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

libs:
	$(MAKE) -C $(LIB_DIR)

fclean:
	$(MAKE) -C $(LIB_DIR) fclean
	rm -rf $(OBJECTS) $(NAME)
