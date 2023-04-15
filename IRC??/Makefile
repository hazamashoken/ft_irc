NAME=ircserv

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -g -O2 -std=c++98

OBJS = $(shell find . -name "*.cpp" -type f | sed 's/\.cpp/\.o/g')

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
