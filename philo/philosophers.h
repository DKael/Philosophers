/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 20:36:03 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/09 16:46:04 by hyungdki         ###   ########.fr       */
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

typedef struct s_philo
{
	int				idx;
	int				first_pick;
    int				second_pick;
	struct timeval	last_eat;
	t_arg			*arg;
}	t_philo;

typedef struct s_arg
{
	int	philo_num;
	int	d_time;
	int	e_time;
	int	s_time;
	int	eat_cnt;
	pthread_t		*philo;
	pthread_mutex_t	*fork;
	struct timeval	start;
	struct timeval	now;
	t_bool	s_flag;
	int	d_flag;
}	t_arg;

void	err_init(char *argv);
void	err_msg(const char *msg, int exit_code);
void	arg_free(t_arg *data);
void* thread_function(void* arg);

void	arg_init(t_arg *data, int argc, char **argv);
int		ft_atoi_int(const char *str);
t_bool	ft_isdecimal(char *str);
char	*ft_itoa(int n);
void	*ft_calloc(size_t count, size_t size);
# endif