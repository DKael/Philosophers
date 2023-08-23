/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 16:17:11 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/23 16:30:10 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static int	make_philo_process(t_arg *arg);
static int	mutexes_init(t_arg *arg);
static int	philosopher_end(t_arg *arg);
static int	main_thread_end(t_arg *arg, int idx, const char *msg);

void	sem_wait_nointr(sem_t *sem)
{
	int	result;

	result = sem_wait(sem);
	while (result == -1)
		result = sem_wait(sem);
}

int	philosopher_start(int argc, char **argv)
{
	t_arg	*arg;

	arg = (t_arg *)malloc(sizeof(t_arg));
	if (arg == T_NULL)
		return (1);
	if (arg_init(arg, argc, argv) != 0)
		return (1);
	arg->start_flag = sem_open("start_flag", O_CREAT, 0644, 1);
	if (arg->start_flag == SEM_FAILED)
		return (arg_free(arg));
	arg->start_flag_chk = TRUE;
	sem_wait_nointr(arg->start_flag);
	arg->end_flag_sem = sem_open("end_flag_sem", O_CREAT, 0644, 1);
	if (arg->end_flag_sem == SEM_FAILED)
	{
		sem_close(arg->start_flag);
		sem_unlink("start_flag");
		return (arg_free(&arg));
	}
	arg->end_flag_sem_chk = TRUE;
	return (make_philo_process(&arg));
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
		arg->philo[idx].philo_pid = fork();
		if (arg->philo[idx].philo_pid == -1)
			;
		else if (arg->philo[idx].philo_pid == 0)
			break;
	}
	if (pthread_create(&(arg->print_thrd), T_NULL,
			print_thread_func, arg) != 0)
		return (main_thread_end(arg, arg->philo_num,
				"pthread create error!"));
	if (pthread_create(&(arg->time_thrd), T_NULL,
			time_thread_func, arg) != 0)
		return (main_thread_end(arg, arg->philo_num + 1,
				"pthread create error!"));
	return (mutexes_init(arg));
}

static int	mutexes_init(t_arg *arg)
{
	if (arg_mutexes_init(arg->fork, arg->philo_num, &arg->fork_cnt) != 0
		|| arg_mutexes_init(arg->last_eat_mtx,
			arg->philo_num, &arg->last_eat_mtx_cnt) != 0
		|| arg_mutexes_init(arg->log_mtx,
			arg->philo_num, &arg->log_mtx_cnt) != 0
		|| gettimeofday(&(arg->start), T_NULL) != 0)
		return (main_thread_end(arg, arg->philo_num + 2,
				"pthread mutex init error!"));
	return (philosopher_end(arg));
}

static int	philosopher_end(t_arg *arg)
{
	int	idx;

	pthread_mutex_unlock(&(arg->start_flag));
	idx = -1;
	while (++idx < arg->philo_num)
		pthread_join(arg->philo[idx].thrd, T_NULL);
	if (check_end_flag(arg) == NORMAL)
	{
		pthread_mutex_lock(&arg->end_flag_mtx);
		arg->end_flag = END;
		pthread_mutex_unlock(&arg->end_flag_mtx);
	}
	pthread_join(arg->philo[idx].thrd, T_NULL);
	pthread_join(arg->philo[idx + 1].thrd, T_NULL);
	arg_mutexes_destroy(arg);
	philos_log_clear(arg, arg->philo_num);
	arg_free(arg);
	return (0);
}

static int	main_thread_end(t_arg *arg, int idx, const char *msg)
{
	sem_wait(arg->end_flag_sem);
	arg->end_flag = ABORT;
	sem_post(arg->end_flag_sem);
	sem_post(arg->start_flag);
	arg_pthreads_join(arg, idx);
	philos_log_clear(arg, idx);
	arg_mutexes_destroy(arg);
	arg_free(arg);
	return (err_msg(arg, msg, 1));
}
