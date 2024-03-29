
CC = cc

CFLAGS = -Wall -Wextra -Werror

MAIN = main.c

SRCS = arr.c free.c list.c outils.c split.c args.c process.c

SRCS_BNS = main_bonus.c get_next_line.c get_next_line_utils.c

OBJS = ${SRCS:.c=.o}

OBJS_MAIN = ${MAIN:.c=.o}

OBJS_BNS = ${SRCS_BNS:.c=.o}

NAME = pipex

NAME_BONUS = pipex_bonus

all : ${NAME}

${NAME} : ${OBJS} ${OBJS_MAIN}
		${CC} ${CFLAGS} ${OBJS_MAIN} ${OBJS} -o ${NAME}

.o:.c pipex.h get_next_line.h pipex_bonus.h
	${CC} ${CFLAGS} $< -o $@

bonus : ${NAME_BONUS}

${NAME_BONUS} : ${OBJS_BNS} ${OBJS}
				${CC} ${CFLAGS} ${OBJS_BNS} ${OBJS} -o ${NAME_BONUS}

clean :
		rm -rf ${OBJS} ${OBJS_MAIN} ${OBJS_BNS}

fclean : clean
		rm -rf ${NAME}

re : fclean all
