# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/03 20:30:42 by hyungdki          #+#    #+#              #
#    Updated: 2023/07/30 21:48:09 by hyungdki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS =	main.c
		
OBJS = 	${SRCS:.c=.o}

CC = cc

CFLAGS = -Wall -Wextra -Werror

NAME = philo

${NAME} : ${TOTAL_OBJS}
	${CC} ${TOTAL_OBJS}  -o $@

%.o :%.c
	${CC} ${CFLAGS} -c -I. $< -o $@

all : ${NAME}

clean:
	rm -f ${OBJS}

fclean: 
	make clean
	rm -rf ${NAME}

re: 
	make fclean
	make all

.PHONY: all clean fclean re bonus
