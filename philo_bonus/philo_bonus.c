/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 16:17:11 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/25 19:04:28 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static int	make_philo_process(t_arg *arg);
static int	sems_init(t_arg *arg);
static int	philosopher_end(t_arg *arg);
static int	main_process_end(t_arg *arg, int idx, const char *msg);

void	sem_wait_nointr(sem_t *sem)
{
	int	result;

	result = sem_wait(sem);
	while (result == -1)
	{
		printf("test, errno : %d\n", errno);
		sleep(1);
		result = sem_wait(sem);
	}
		
}

void	ft_sem_destroy(t_csem *csem)
{
	sem_close(csem->sem);
	sem_unlink(csem->name);
}

sem_t	*ft_sem_open(const char *name, mode_t mode, unsigned int value)
{
	sem_t	*temp;

	temp = sem_open(name, O_EXCL | O_CREAT, mode, value);
	if (temp == SEM_FAILED)
	{
		sem_unlink(name);
		temp = sem_open(name, O_EXCL | O_CREAT, mode, value);
	}
	return (temp);		
}

int	philosopher_start(int argc, char **argv)
{
	t_arg	*arg;

	arg = (t_arg *)malloc(sizeof(t_arg));
	if (arg == T_NULL)
		return (1);
	if (arg_init(arg, argc, argv) != 0)
		return (1);
	arg->start_flag.name = "start_flag";
	arg->start_flag.sem = ft_sem_open("start_flag", 0644, 1);
	
	if (arg->start_flag.sem == SEM_FAILED)
	{
		printf("fail test0-1\n");
		return (main_process_end(arg, 0, "sem open failed!"));
	}
	arg->start_flag_chk = TRUE;
	sem_wait_nointr(arg->start_flag.sem);
	arg->end_flag_sem.name = "end_flag_sem";
	arg->end_flag_sem.sem = ft_sem_open("end_flag_sem", 0644, 1);
	if (arg->end_flag_sem.sem == SEM_FAILED)
		return (main_process_end(arg, 0, "sem open failed!"));
	arg->end_flag_sem_chk = TRUE;
	return (make_philo_process(arg));
}

static int	make_philo_process(t_arg *arg)
{
	int	idx;

	idx = -1;
	while (++idx < arg->philo_num)
	{
		arg->philo[idx].idx = idx;
		dll_init(&(arg->philo[idx].logs));
		arg->philo[idx].arg = arg;
		arg->philo[idx].end = FALSE;
		arg->philo[idx].eat_cnt = 0;
		arg->philo[idx].philo_pid = fork();
		if (arg->philo[idx].philo_pid == -1)
			return (main_process_end(arg, idx, "fork() error!"));
		else if (arg->philo[idx].philo_pid == 0)
		{
			philo_thread_func(arg, &arg->philo[idx]);
			exit(0);
		}
	}
	if (pthread_create(&(arg->print_thrd), T_NULL,
			print_thread_func, arg) != 0)
		return (main_process_end(arg, arg->philo_num,
				"pthread create error!"));
	arg->print_thrd_chk = TRUE;
	return (sems_init(arg));
}

static int	sems_init(t_arg *arg)
{
	if (pthread_create(&(arg->time_thrd), T_NULL,
			time_thread_func, arg) != 0)
		return (main_process_end(arg, arg->philo_num,
				"pthread create error!"));
	arg->time_thrd_chk = TRUE;
	arg->fork.name = "fork";
	arg->fork.sem = ft_sem_open(arg->fork.name, 0644, arg->philo_num);
	if (arg->fork.sem == SEM_FAILED)
		return (main_process_end(arg, arg->philo_num, "sem open failed!"));
	arg->fork_chk = TRUE;
	if (sems_open(arg->last_eat_sem, arg->philo_num,
			&arg->last_eat_sem_cnt, "last_eat_sem") != 0
		|| sems_open(arg->log_sem, arg->philo_num,
			&arg->log_sem_cnt, "log_sem") != 0
		|| gettimeofday(&(arg->start), T_NULL) != 0)
		return (main_process_end(arg, arg->philo_num,
				"sem open failed!"));
	return (philosopher_end(arg));
}

static int	philosopher_end(t_arg *arg)
{
	int	idx;

	sem_post(arg->start_flag.sem);
	idx = -1;
	while (++idx < arg->philo_num)
		waitpid(arg->philo[idx].philo_pid, T_NULL, 0);
	if (check_end_flag(arg) == NORMAL)
	{
		sem_wait_nointr(arg->end_flag_sem.sem);
		arg->end_flag = END;
		sem_post(arg->end_flag_sem.sem);
	}
	pthread_join(arg->print_thrd, T_NULL);
	pthread_join(arg->time_thrd, T_NULL);
	philos_log_clear(arg, arg->philo_num);
	arg_sems_destroy(arg);
	arg_free(arg);
	return (0);
}

static int	main_process_end(t_arg *arg, int idx, const char *msg)
{
	sem_wait_nointr(arg->end_flag_sem.sem);
	arg->end_flag = ABORT;
	sem_post(arg->end_flag_sem.sem);
	sem_post(arg->start_flag.sem);
	arg_waitpids(arg, idx);
	if (arg->print_thrd_chk == TRUE)
		pthread_join(arg->print_thrd, T_NULL);
	if (arg->time_thrd_chk == TRUE)
		pthread_join(arg->time_thrd, T_NULL);
	philos_log_clear(arg, idx);
	arg_sems_destroy(arg);
	arg_free(arg);
	return (err_msg(arg, msg, 1));
}
