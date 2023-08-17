# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/03 20:30:42 by hyungdki          #+#    #+#              #
#    Updated: 2023/08/17 09:19:39 by hyungdki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS =	main.c \
		double_linked_list1.c \
		double_linked_list2.c \
		double_linked_list3.c \
		error.c \
		init.c \
		philo_thread_func.c \
		print_thread_func.c \
		quick_sort.c \
		time_thread_func.c\
		util.c \
		
OBJS = 	${SRCS:.c=.o}

TOTAL_OBJS = ${OBJS}

CC = cc

CFLAGS = -Wall -Wextra -Werror

NAME = philo

${NAME} : ${TOTAL_OBJS}
	${CC} ${CFLAGS} ${TOTAL_OBJS}  -o $@

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
