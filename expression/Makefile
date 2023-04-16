SRCS	= main.cpp Expression.cpp Message.cpp Split.cpp

OBJS	= ${SRCS:.cpp=.o}

NAME	= extest

CXX		= c++

CXFLAGS	= -Wall -Wextra -Werror -std=c++98

RM		= rm -f

MAKE	= make

all:	${NAME}

bonus:	${NAME}

.cpp.o:
	${CXX} ${CXFLAGS} -c $< -o ${<:.cpp=.o}

${NAME}:	${OBJS}
	${CXX} -o ${NAME} ${OBJS}

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME} ${STDNAME} ${FTNAME}

re:	fclean all

.PHONY:	all bonus clean fclean re
