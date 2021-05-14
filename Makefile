SRCS = ft_printf.c \
		input.c \
		int_print.c \
		printf_utils.c \
		string_print.c \
		unsigned_print.c

OBJS = ${SRCS:%.c=%.o}

NAME = libftprintf.a

${NAME}: ${OBJS}
	ar rc ${NAME} ${OBJS}

%.o: %.c
	gcc -Wall -Wextra -Werror -c $< -o $@

all: ${NAME}

clean:
	rm -f ${OBJS}

fclean:
	rm -f ${OBJS} ${NAME}

re:	fclean all

.PHONY: all clean fclean re
