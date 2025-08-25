NAME = ircserv

SRC = main.cpp Server.cpp Client.cpp Channel.cpp join.cpp privmsg.cpp

OBJ = main.o Server.o Client.o Channel.o join.o

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