/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 20:36:03 by hyungdki          #+#    #+#             */
/*   Updated: 2023/07/30 23:10:02 by hyungdki         ###   ########.fr       */
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
#include <limits.h>

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
	int	philo_num;
	int	d_time;
	int	e_time;
	int	s_time;
	int	eat_cnt;
	pthread_t	*philo;
	int			*fork;
	pthread_mutex_t mutex;
	int		cur_idx;
}	t_philo;

typedef struct s_pass
{
	int		idx;
	t_bool	left_first;
	t_philo	*data;
}	t_pass;

void	err_init(char *argv);
void	err_msg(const char *msg, int exit_code);
void	philo_free(t_philo *data);

void	arg_init(t_philo *data, int argc, char **argv, int idx);
void	philo_init(t_philo *data);
int		ft_atoi_int(const char *str);
# endif