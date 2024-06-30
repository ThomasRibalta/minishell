NAME = minishell
CC = gcc
RM = rm -f
FLAGS =

SRC_1 = src/utils/parser/parser.c \
				src/utils/parser/expenser.c\
				src/utils/parser/parser_utils.c\
				src/utils/parser/utils3.c\
				src/utils/parser/utils2.c\
				src/utils/parser/utils.c\
				src/utils/parser/redirection.c\
				src/utils/parser/ast_gestion.c\
				src/utils/parser/token_gestion.c\
				src/utils/wildcards/wildcards.c \
				src/utils/wildcards/wildcards1.c \
				src/utils/wildcards/wildcards3.c \
				src/utils/wildcards/wildcards4.c \
				src/utils/init_terminal.c \
				src/utils/init_terminal2.c \
				src/utils/exec/executer.c \
				src/utils/exec/normal_exec.c \
				src/utils/exec/parenthese_exec.c \
				src/utils/exec/redirection.c \
				src/utils/exec/utils.c \
				src/utils/exec/free_parser.c \
				src/utils/exec/execv.c \
				src/utils/lexer/lexer.c \
				src/utils/lexer/token_manager.c \
				src/utils/lexer/lexer2.c \
				src/utils/lexer/symbols.c \
				src/utils/lexer/gestion_erreur.c \
				src/utils/prompt_utils.c \
				src/utils/prompt_utils2.c \
				src/utils/signal.c \
				src/utils/builtins/cd.c \
				src/utils/builtins/echo.c \
				src/utils/builtins/env.c \
				src/utils/builtins/exit.c \
				src/utils/builtins/export.c \
				src/utils/builtins/export_utils.c \
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
