/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 20:36:03 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/13 18:06:01 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <memory.h>
# include <unistd.h>
# include <sys/time.h>
# include <limits.h>
# include "double_linked_list.h"

# if !defined(TRUE) && !defined(FALSE)
#  define TRUE 1
#  define FALSE 0
# endif
# ifndef T_NULL
#  define T_NULL (void *)0
# endif

typedef int	t_bool;
typedef	struct timeval t_timeval;

typedef enum e_thread_status
{
	DEATH = 1,
	ABORT = 2
}	t_thread_status;

typedef	enum e_philo_status
{
	GET_FORK,
	EATING,
	SLEEPING,
	THINKING,
	DIE
}	t_philo_status;

typedef struct s_log
{
	t_timeval	time;
	int			who;
	t_philo_status	status;
}	t_log;

typedef struct s_philo
{
	int				idx;
	pthread_mutex_t	*first_fork;
    pthread_mutex_t	*second_fork;
	t_timeval	last_eat;
	pthread_t		thrd;
	t_dll			logs;
	t_arg			*arg;
}	t_philo;

typedef struct s_arg
{
	int	philo_num;
	int	d_time;
	int	e_time;
	int	s_time;
	int	have_to_eat;
	t_philo			*philo;
	pthread_mutex_t	*fork;
	pthread_t	print_thrd;
	pthread_t	time_thrd;
	t_timeval	start;
	t_timeval	now;
	t_bool	start_flag;
	int	da_flag;
}	t_arg;

void	err_init(char *argv);
void	err_msg(const char *msg);
void	arg_free(t_arg *data);
void* thread_function(void* arg);

int	arg_init(t_arg *data, int argc, char **argv);
int		ft_atoi_int(const char *str);
t_bool	ft_isdecimal(char *str);
char	*ft_itoa(int n);
void	*ft_calloc(size_t count, size_t size);
# endif