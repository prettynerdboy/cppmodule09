NAME		=	RPN
CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98
INCLUDE		=	-I includes
RM			=	rm -rf

SRCS		=	$(wildcard *.cpp)

OBJ_D 		=	objs/
OBJ			=	$(SRCS:.cpp=)
OBJS		=	$(addprefix $(OBJ_D), $(addsuffix .o, $(OBJ)))

$(OBJ_D)%.o :%.cpp
			mkdir -p $(OBJ_D)
			$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

all:		$(NAME)

$(NAME):	$(OBJS)
			$(CXX) $(CXXFLAGS) $(INCLUDE) $(OBJS) -o $(NAME)

clean:
			$(RM) $(OBJ_D)

fclean: 	clean
			$(RM) $(NAME)

re: 		fclean all

.PHONY:		all clean fclean re