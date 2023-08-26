/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 19:25:33 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/26 21:17:42 by hyungdki         ###   ########.fr       */
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


#include "errno.h"


# if !defined(TRUE) && !defined(FALSE)
#  define TRUE 1
#  define FALSE 0
# endif
# ifndef T_NULL
#  define T_NULL (void *)0
# endif

# define S_TO_US 1000000
# define MS_TO_US 1000

# define FT_WIFEXITED(x) (((*(int *)&(x)) & 0177) == 0)

typedef int				t_bool;

typedef struct timeval	t_timeval;

typedef enum e_thread_status
{
	NORMAL,
	ABORT,
	PHILO_DIE,
	END
}	t_thread_status;

typedef enum e_philo_status
{
	THINKING,
	GET_FORK,
	EATING,
	SLEEPING,
	DIE,
	EAT_DONE
}	t_philo_status;

typedef	struct s_csem
{
	sem_t	*sem;
	char	*name;
}	t_csem;


typedef struct s_arg
{
	int				philo_num;
	int				d_time;
	int				e_time;
	int				s_time;
	int				have_to_eat;
	
	t_csem			fork;
	t_bool			fork_chk;
	
	
	t_csem			start_flag;
	t_bool			start_flag_chk;
	t_csem			print_sem;
	t_bool			print_sem_chk;
	
	pid_t	*pid_lst;
	
	t_timeval		start;
	char			*program_name;
}	t_arg;

typedef struct s_philo
{
	int				idx;
	long			last_eat;
	t_csem			last_eat_sem;
	t_bool			last_eat_sem_chk;
	t_thread_status	end_flag;
	t_csem			end_flag_sem;
	t_bool			end_flag_sem_chk;
	t_bool			end;
	int				eat_cnt;
	pthread_t		time_thrd;
	t_bool			time_thrd_chk;
}	t_philo;

// error.c
int		err_msg(t_arg *arg, const char *msg, int return_code);
void	*thread_error_end(t_arg *arg);
int		check_end_flag(t_arg *arg);
// init.c
int		arg_init(t_arg *data, int argc, char **argv);
int	sems_open(t_csem *lst, int num, int *sem_cnt, const char *name);
// philo_report.c
t_bool	report(t_philo *value, t_philo_status status, t_arg *arg);
t_bool	die_report(t_arg *arg, long time_lapse_usec, int idx);
// philo_thread_func.c
// philo.c
int		philosopher_start(int argc, char **argv);
void	*philo_process_func(t_arg *arg, int idx);

// resource_free_func.c

void	kill_and_waitpid(t_arg *arg, int cnt);
void	arg_sems_destroy(t_arg *arg);
// time_thread_func.c
void	*time_thread_func(void *arg);
//util.c
int		ft_atoi_int(const char *str);
t_bool	ft_isdecimal(char *str);
void	*ft_calloc(size_t count, size_t size);
t_bool	ft_usleep(long us);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_itoa(int n);
size_t	ft_strlen(const char *s);

void	sem_wait_nointr(sem_t *sem);
void	ft_sem_destroy(t_csem *csem);
sem_t	*ft_sem_open(const char *name, mode_t mode, unsigned int value);
#endif