/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 19:25:33 by hyungdki          #+#    #+#             */
/*   Updated: 2023/11/30 18:47:20 by hyungdki         ###   ########.fr       */
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
# include "ft_errno.h"
# include "quick_sort.h"

# if !defined(TRUE) && !defined(FALSE)
#  define TRUE 1
#  define FALSE 0
# endif
# ifndef T_NULL
#  define T_NULL (void *)0
# endif

# define S_TO_US 1000000
# define MS_TO_US 1000

typedef int				t_bool;

typedef struct timeval	t_timeval;

typedef enum e_thread_status
{
	NORMAL,
	ABORT,
	PHILO_DIE,
	END
}	t_thread_status;
typedef struct s_arg
{
	int				philo_num;
	int				d_time;
	int				e_time;
	int				s_time;
	int				have_to_eat;
	struct s_philo	*philo;
	pthread_mutex_t	*fork;
	int				fork_cnt;
	pthread_mutex_t	*last_eat_mtx;
	int				last_eat_mtx_cnt;
	pthread_mutex_t	*log_mtx;
	int				log_mtx_cnt;
	pthread_mutex_t	start_flag;
	t_bool			start_flag_chk;
	pthread_mutex_t	end_flag_mtx;
	t_bool			end_flag_mtx_chk;
	t_thread_status	end_flag;
	t_timeval		start;
	char			*program_name;
}	t_arg;

typedef struct s_philo
{
	int				idx;
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;
	long			last_eat;
	t_bool			end;
	pthread_t		thrd;
	t_dll			logs;
	t_arg			*arg;
	int				eat_cnt;
}	t_philo;

typedef enum e_philo_status
{
	THINKING,
	GET_FORK,
	EATING,
	SLEEPING,
	DIE,
	EAT_DONE
}	t_philo_status;

typedef struct s_log
{
	t_timeval		time;
	long			usec;
	int				who;
	t_philo_status	status;
}	t_log;

typedef struct s_srt
{
	long			usec;
	t_philo_status	status;
	t_dllnode		*ptr;
}	t_srt;

// error.c
int		err_msg(t_arg *arg, const char *msg, int return_code);
void	*thread_error_end(t_arg *arg);
int		check_end_flag(t_arg *arg);
// init.c
int		arg_init(t_arg *data, int argc, char **argv);
int		arg_mutexes_init(pthread_mutex_t *lst, int num, int *mtx_cnt);
// philo_report.c
t_bool	report(t_philo *value, t_philo_status status, t_arg *arg);
// main_thread.c
int		philosopher_start(int argc, char **argv);
// philo_thread_func.c
void	*philo_thread_func(void *arg);
// print_thread_func.c
void	*print_thread_func(void *input_arg);
// resource_free_func.c
void	arg_free(t_arg *data);
void	log_delete_func(void *content);
void	philos_log_clear(t_arg *arg, int cnt);
void	arg_pthreads_join(t_arg *arg, int cnt);
void	arg_mutexes_destroy(t_arg *arg);
// time_thread_func.c
void	*time_thread_func(void *arg);
//util.c
int		ft_atoi_int(const char *str);
t_bool	ft_isdecimal(char *str);
void	*ft_calloc(size_t count, size_t size);
t_bool	ft_usleep(long us, t_arg *arg);

#endif