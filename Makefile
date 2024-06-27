NAME		:= webserv

HEADER		:= ./inc/

SRC_PATH	:= ./src/
SRC_FILES	:= main.cpp Cluster.cpp Server.cpp Socket.cpp Request.cpp Response.cpp
SRC			:= $(addprefix $(SRC_PATH),$(SRC_FILES))

OBJ_PATH	:= objs/
OBJ			:= $(addprefix $(OBJ_PATH),$(SRC:$(SRC_PATH)%.cpp=%.o))
DEP			:= $(addprefix $(OBJ_PATH),$(SRC:$(SRC_PATH)%.cpp=%.d))

CPP			:= g++ -std=c++98

#CPPFLAGS	:= -Wall -Wextra -Werror -MMD -Wshadow -Wno-shadow

RM			:= @rm -rf


all:	$(NAME)

$(NAME): $(OBJ)
	${CPP} ${CPPFLAGS} ${OBJ} -o ${NAME}
	@echo "$(PINK)Webserv compiled$(DEF_COLOR)"

-include $(DEP)

$(OBJ_PATH)%.o:	$(SRC_PATH)%.cpp Makefile
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) -I$(HEADER) -c $< -o $@	

clean:
	$(RM) $(OBJ_PATH) > /dev/null
	@echo "$(PINK)Objects removed$(DEF_COLOR)"

fclean: clean
	$(RM) $(NAME) > /dev/null
	@echo "$(PINK)Webserv removed$(DEF_COLOR)"

re:	fclean all

.PHONY: all clean fclean re
