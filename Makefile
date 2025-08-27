NAME = ircserv

SRC = main.cpp Server.cpp Client.cpp Channel.cpp join.cpp privmsg.cpp fIleTransfer.cpp

# OBJ = main.o Server.o Client.o Channel.o join.o privmsg.o
OBJ = ${SRC:.cpp=.o}

CXX = c++

CXXFLAGS = -Wextra  -Wall -std=c++98 -Werror #-fsanitize=address -g3

$(NAME) : $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)

all : $(NAME)

clean :
	rm -rf $(OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean all