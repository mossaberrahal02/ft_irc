NAME = irc

SRC = main.cpp Server.cpp Client.cpp Channel.cpp

OBJ = main.o Server.o Client.o Channel.o

CXX = c++

CXXFLAGS = -Wextra -Werror -Wall -std=c++98

$(NAME) : $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)

all : $(NAME)

clean :
	rm -rf $(OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean all