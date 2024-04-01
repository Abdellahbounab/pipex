
CC = cc

CFLAGS = -Wall -Wextra -Werror

SRCS = outils.c free.c list.c split.c args.c process.c arr.c main.c

SRCS_BNS = get_next_line_bonus.c outils_bonus.c free_bonus.c \
		list_bonus.c split_bonus.c args_bonus.c \
		process_bonus.c extra_outils_bonus.c \
		arr_bonus.c main_bonus.c

OBJS = ${SRCS:.c=.o}

OBJS_BNS = ${SRCS_BNS:.c=.o}

NAME = pipex

NAME_BONUS = pipex_bonus

all : ${NAME}

${NAME} : ${OBJS}
		@${CC} ${CFLAGS} ${OBJS} -o ${NAME} 
		@echo "\033[32mcompilation : ./pipex\033[0m"

%.o:%.c pipex.h pipex_bonus.h
		@${CC} ${CFLAGS} -c $< -o $@

bonus : ${NAME_BONUS}

${NAME_BONUS} : ${OBJS_BNS}
				@${CC} ${CFLAGS} ${OBJS_BNS} -o ${NAME_BONUS} 
				@echo "\033[32mcompilation : ./pipex_bonus\033[0m"
				
clean :
		@rm -rf ${OBJS} ${OBJS_MAIN} ${OBJS_BNS}
		@echo "\033[34m*.o files deleted\033[0m"

fclean : clean
		@rm -rf ${NAME} ${NAME_BONUS}

re : fclean all
