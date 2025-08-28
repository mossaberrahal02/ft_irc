NAME = ircserv

SRC = main.cpp Server.cpp Client.cpp Channel.cpp join.cpp authentification.cpp topic.cpp\
		kick.cpp invite.cpp mode.cpp privmsg.cpp fIleTransfer.cpp file.cpp Bot.cpp part.cpp

OBJ = $(SRC:.cpp=.o)

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