/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 19:25:33 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/27 18:45:03 by hyungdki         ###   ########.fr       */
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

# if !defined(TRUE) && !defined(FALSE)
#  define TRUE 1
#  define FALSE 0
# endif
# ifndef T_NULL
#  define T_NULL (void *)0
# endif

# define S_TO_US 1000000
# define MS_TO_US 1000
# define BUF_SIZE 128

typedef int				t_bool;

typedef struct timeval	t_timeval;

typedef enum e_philo_status
{
	THINKING,
	GET_FORK,
	EATING,
	SLEEPING,
	DIE,
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
	pid_t		*pid_lst;
	t_timeval	start;
	char		*program_name;
}	t_arg;

typedef struct s_philo
{
	int				idx;
	t_timeval		last_eat;
	int				eat_cnt;
}	t_philo;

//custom_sem_func_bonus.c
void	sem_wait_nointr(sem_t *sem);
void	ft_sem_destroy(t_csem *csem);
sem_t	*ft_sem_open(const char *name, mode_t mode, unsigned int value);
// error_bonus.c
int		err_msg(t_arg *arg, const char *msg, int return_code);
// init_bonus.c
int		arg_init(t_arg *data, int argc, char **argv);
// main_process_bonus.c
int		philosopher_start(int argc, char **argv);
// philo_process_bonus.c
void	philo_process_func(t_arg *arg, int idx);
// resource_free_func.c
void	kill_and_waitpid(t_arg *arg, int cnt);
void	arg_sems_destroy(t_arg *arg);
//util1_bonus.c
int		ft_atoi_int(const char *str);
t_bool	ft_isdecimal(char *str);
t_bool	ft_usleep(long us, t_timeval last_eat, int d_time);
long	time_calc(t_timeval t1, t_timeval t2);
//util2_bonus.c
int		ft_itoa(int n, char *buf);
void	ft_strcat(char *dest, char *src);
size_t	ft_strlen(const char *s);
//wait_bonus.c
int		ft_wifexited(int status);
int		ft_wexitstatus(int status);
int		ft_wifsignaled(int status);

#endif