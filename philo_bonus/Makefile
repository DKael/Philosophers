# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/03 20:30:42 by hyungdki          #+#    #+#              #
#    Updated: 2023/08/27 16:20:00 by hyungdki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS =	main_bonus.c \
		custom_sem_func_bonus.c \
		error_bonus.c \
		init_bonus.c \
		main_process_bonus.c \
		philo_process_bonus.c \
		resource_free_func_bonus.c \
		util1_bonus.c \
		util2_bonus.c \
		wait_bonus.c
		
OBJS = 	${SRCS:.c=.o}

TOTAL_OBJS = ${OBJS}

CC = cc

CFLAGS = -Wall -Wextra -Werror

NAME = philo_bonus

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
