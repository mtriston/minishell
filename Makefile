COM_COLOR   = \033[0;34m
OBJ_COLOR   = \033[0;36m
OK_COLOR    = \033[0;32m
ERROR_COLOR = \033[0;31m
WARN_COLOR  = \033[0;33m
NO_COLOR    = \033[m

OK_STRING    = [OK]     
ERROR_STRING = [ERROR]
WARN_STRING  = [WARNING]
COM_STRING   = Compiling

NAME = minishell

SRC_DIR = src/
OBJ_DIR = bin/
HDR_DIR = includes/

INCLUDES = -I$(HDR_DIR)

CC = gcc
FLAGS = -Wall -Werror -Wextra -g

LEXER_DIR = lexer/
LEXER_FILES = $(addsuffix .c, $(addprefix $(LEXER_DIR),\
			lexer\
            lexer_utils\
			prepare_line\
			read_line\
			token_list_utils\
			validate_line\
			))

PARSER_DIR = parser/
PARSER_FILES = $(addsuffix .c, $(addprefix $(PARSER_DIR),\
			parser\
			cmd_list_utils\
			parse_redirects\
			split_pipe\
			))

EXECUTOR_DIR = executor/
EXECUTOR_FILES = $(addsuffix .c, $(addprefix $(EXECUTOR_DIR),\
			executor\
			executor_utils\
			launch_executable\
			))

BUILTINS_DIR = builtins/
BUILTINS_FILES = $(addsuffix .c, $(addprefix $(BUILTINS_DIR),\
			builtins\
			export\
			export_utils\
			unset\
			))

OTHER_FILES = $(addsuffix .c,\
			main\
			error\
			signals\
			)

SRC_FILES = $(LEXER_FILES) $(PARSER_FILES) $(EXECUTOR_FILES) $(BUILTINS_FILES) $(OTHER_FILES)

OBJ_FILES = $(addprefix $(OBJ_DIR), $(SRC_FILES:.c=.o))

all: $(OBJ_DIR) $(NAME)

$(NAME): $(OBJ_FILES)
	@ make -C ./libs/libft
	@ $(CC) $(FLAGS) $(OBJ_FILES) -o $(NAME) -L./libs/libft -lft
	@echo "$(OK_COLOR) $(OK_STRING) $(OBJ_COLOR) $(NAME) $(NO_COLOR)"

$(OBJ_FILES): $(OBJ_DIR)%.o : $(SRC_DIR)%.c
	@$(CC) $(FLAGS) -c $< -o $@
	@echo "$(COM_COLOR) $(COM_STRING) $(OBJ_COLOR) $(@) $(NO_COLOR)"

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)
	@mkdir $(addprefix $(OBJ_DIR), $(LEXER_DIR) $(PARSER_DIR) $(EXECUTOR_DIR) $(BUILTINS_DIR))

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(WARN_COLOR) All object files have been removed $(NO_COLOR)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(WARN_COLOR) $(NAME) has been removed $(NO_COLOR)"

re: fclean all
