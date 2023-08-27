# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/03 20:30:42 by hyungdki          #+#    #+#              #
#    Updated: 2023/08/27 16:20:28 by hyungdki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS =	main.c \
		double_linked_list1.c \
		double_linked_list2.c \
		double_linked_list3.c \
		error.c \
		init.c \
		main_thread.c \
		philo_report.c \
		philo_thread_func.c \
		print_thread_func1.c \
		print_thread_func2.c \
		quick_sort.c \
		resource_free_func.c \
		srt_func.c \
		time_thread_func.c\
		util.c \
		
OBJS = 	${SRCS:.c=.o}

TOTAL_OBJS = ${OBJS}

CC = cc

CFLAGS = -Wall -Wextra -Werror

NAME = philo

DATA_RACE_CHECK = 0

ADDRESS_CHECK = 0

ifeq ($(DATA_RACE_CHECK),1)
${NAME} : ${TOTAL_OBJS}
	${CC} ${CFLAGS} ${TOTAL_OBJS} -g -fsanitize=thread -o $@
else ifeq ($(ADDRESS_CHECK),1)
${NAME} : ${TOTAL_OBJS}
	${CC} ${CFLAGS} ${TOTAL_OBJS} -g -fsanitize=address -o $@
else
${NAME} : ${TOTAL_OBJS}
	${CC} ${CFLAGS} ${TOTAL_OBJS} -o $@
endif

%.o :%.c
	${CC} ${CFLAGS} -c -I. $< -o $@

all : ${NAME}

data_race_check : 
	@make DATA_RACE_CHECK=1

address_check : 
	@make ADDRESS_CHECK=1

clean:
	rm -f ${OBJS}

fclean: 
	make clean
	rm -rf ${NAME}

re: 
	make fclean
	make all

.PHONY: all clean fclean re bonus data_race_check
