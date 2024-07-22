RED = \033[1;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
BLACK = \033[0;39m

NAME = webserv
CPP = g++ -g -O0
FLAGS = -Wall -Wextra -Werror -MMD -std=c++98 -Wshadow -Wno-shadow -fsanitize=address
RM = rm -f

SRC =  main.cpp ServerConfig.cpp Parse.cpp ParseDir.cpp LocationConfig.cpp \
Utils.cpp Cluster.cpp Server.cpp Cgi.cpp Socket.cpp Request.cpp Response.cpp 
F_SRC = src/
F_OBJ = .obj/
OBJ = $(addprefix $(F_OBJ), $(SRC:.cpp=.o))
DEP = $(addprefix $(F_OBJ), $(SRC:.cpp=.d))

$(F_OBJ)%.o: $(F_SRC)%.cpp Makefile
	@$(CPP) $(FLAGS) -I ./inc/ -c $< -o $@

all: dir $(NAME)

dir:
	@mkdir -p $(F_OBJ)

-include $(DEP)
$(NAME): $(OBJ)
	$(CPP) $(FLAGS) -I ./inc/ $(OBJ) -o $(NAME)
	@echo "$(GREEN)Everything has been compiled.$(BLACK)"

test:
	@${CPP} ${CPPFLAGS} test.cpp
	@./a.out
	@rm -f ./a.out test.d

run: ${NAME}
	./${NAME} | cat -e

clean:
	$(RM) $(OBJ) $(DEP)
	$(RM) -R obj

fclean: clean
	$(RM) $(NAME)
	@echo "$(YELLOW)Everything has been cleaned.$(BLACK)"

re: fclean all

.PHONY: all clean fclean re dir
.SILENT: fclean clean run test dir all ${NAME}
