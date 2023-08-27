/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 19:25:33 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/27 16:17:28 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <memory.h>
# include <unistd.h>
# include <sys/time.h>
# include <limits.h>
# include <semaphore.h>
# include <signal.h>
# include <sys/wait.h>
# include "ft_errno_bonus.h"

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

typedef enum e_philo_status
{
	THINKING,
	GET_FORK,
	EATING,
	SLEEPING,
	DIE,
	EAT_DONE
}	t_philo_status;

typedef struct s_csem
{
	sem_t	*sem;
	char	*name;
}	t_csem;

typedef struct s_arg
{
	int			philo_num;
	int			d_time;
	int			e_time;
	int			s_time;
	int			have_to_eat;
	t_csem		fork;
	t_bool		fork_chk;
	t_csem		start_flag;
	t_bool		start_flag_chk;
	t_csem		print_sem;
	t_bool		print_sem_chk;
	t_csem		*last_eat_sem;
	int			last_eat_sem_cnt;
	pid_t		*pid_lst;
	t_timeval	start;
	char		*program_name;
}	t_arg;

typedef struct s_philo
{
	int				idx;
	long			last_eat;
	int				eat_cnt;
	pthread_t		time_thrd;
	t_bool			time_thrd_chk;
}	t_philo;

typedef struct s_arg_and_philo
{
	t_arg	*arg;
	t_philo	*philo;
}	t_arg_and_philo;

//custom_sem_func_bonus.c
void	sem_wait_nointr(sem_t *sem);
void	ft_sem_destroy(t_csem *csem);
sem_t	*ft_sem_open(const char *name, mode_t mode, unsigned int value);
int		make_multiple_sem(t_csem *lst, const char *name, int *cnt, int num);
// error_bonus.c
int		err_msg(t_arg *arg, const char *msg, int return_code);
// init_bonus.c
int		arg_init(t_arg *data, int argc, char **argv);
// main_process_bonus.c
int		philosopher_start(int argc, char **argv);
// philo_process_bonus.c
void	philo_process_func(t_arg *arg, int idx);
void	philo_report(t_arg *arg, t_philo *data, t_philo_status status);
// philo_time_thread_func_bonus.c
void	*time_thread_func(void *arg);
// resource_free_func.c
void	arg_heap_free(t_arg *arg);
void	kill_and_waitpid(t_arg *arg, int cnt);
void	arg_sems_destroy(t_arg *arg);
//util1_bonus.c
int		ft_atoi_int(const char *str);
t_bool	ft_isdecimal(char *str);
void	*ft_calloc(size_t count, size_t size);
t_bool	ft_usleep(long us);
char	*ft_strjoin(char const *s1, char const *s2);
//util2_bonus.c
char	*ft_itoa(int n);
long	time_calc(t_arg *arg);
size_t	ft_strlen(const char *s);

#endif