CC			= c++ -Wall -Wextra -Werror -std=c++98 #-g -fsanitize=address
RM			= rm -rf
NAME		= ./webserv
NAME_SHORT	= webserv

INC_DIR		= include
MAIN_INC	= -I$(INC_DIR)
INC			= $(shell find $(INC_DIR) -type f -name "*.hpp")

SRC_DIR		= src
SRC			= $(shell find $(SRC_DIR) -type f -name "*.cpp")

OBJ			= $(SRC:.cpp=.o)

_COLOR		= \033[32m
_BOLDCOLOR	= \033[32;1m
_RESET		= \033[0m
_CLEAR		= \033[0K\r\c
_OK			= [\033[32mOK\033[0m]

%.o			: %.cpp
			@echo "[..] $(NAME_SHORT)... compiling $*.cpp\r\c"
			@$(CC) $(MAIN_INC) -c $< -o $@
			@echo "$(_CLEAR)"

all			: $(NAME)

$(NAME)		: $(OBJ) $(INC)
			@$(CC) $(OBJ) $(MAIN_INC) -o $(NAME)
			@echo "$(_OK) $(NAME_SHORT) compiled"

clean		:
			@$(RM) $(OBJ)

fclean		: clean
			@$(RM) $(NAME)

re			: fclean all

.PHONY		: all clean fclean re