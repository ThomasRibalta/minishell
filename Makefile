NAME = minishell
CC = gcc
RM = rm -f
FLAGS = -Wall -Wextra -Werror

SRC_1 = src/utils/parser/expenser.c \
				src/utils/parser/parser.c \
				src/utils/parser/utils.c \
				src/utils/wildcards.c \
				src/utils/init_terminal.c \
				src/utils/executer.c \
				src/utils/lexer.c \
				src/utils/prompt_utils.c \
				src/utils/signal.c \
				src/utils/builtins/cd.c \
				src/utils/builtins/echo.c \
				src/utils/builtins/env.c \
				src/utils/builtins/exit.c \
				src/utils/builtins/export.c \
				src/utils/builtins/pwd.c \
				src/utils/builtins/unset.c \
				src/utils/gnl/get_next_line_utils.c \
				src/utils/gnl/get_next_line.c
			

OBJ_1 = ${SRC_1:.c=.o}

.c.o:
	${CC} ${FLAGS} -c $< -o ${<:.c=.o}

${NAME}: ${OBJ_1}
	make -C src/utils/libft
	${CC} ${FLAGS} ${OBJ_1} main.c -g -o ${NAME} -L src/utils/libft -lft -l readline

all: ${NAME}

clean:
	make -C src/utils/libft clean
	${RM} ${OBJ_1} ${NAME}

fclean: clean
	${RM} ${NAME}
	make -C src/utils/libft fclean

re: clean all

.PHONY: all clean fclean re bonus
