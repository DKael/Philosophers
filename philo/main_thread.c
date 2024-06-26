/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_thread.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 16:17:11 by hyungdki          #+#    #+#             */
/*   Updated: 2024/01/27 17:56:18 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	make_thread(t_arg *arg);
static int	mutexes_init(t_arg *arg);
static int	philosopher_end(t_arg *arg);
static int	main_thread_end(t_arg *arg, int idx, const char *msg);

int	philosopher_start(int argc, char **argv)
{
	t_arg	arg;

	if (arg_init(&arg, argc, argv) != 0)
		return (1);
	if (pthread_mutex_init(&arg.start_flag, T_NULL) != 0)
	{
		arg_free(&arg);
		return (1);
	}
	arg.start_flag_chk = TRUE;
	pthread_mutex_lock(&arg.start_flag);
	if (pthread_mutex_init(&arg.end_flag_mtx, T_NULL) != 0)
	{
		arg_mutexes_destroy(&arg);
		arg_free(&arg);
		return (1);
	}
	arg.end_flag_mtx_chk = TRUE;
	return (make_thread(&arg));
}

inline static int	make_thread(t_arg *arg)
{
	int	idx;

	idx = -1;
	while (++idx < arg->philo_num)
	{
		arg->philo[idx].idx = idx;
		arg->philo[idx].fork[0] = &(arg->fork[idx]);
		arg->philo[idx].fork[1] = &(arg->fork[(idx + 1) % arg->philo_num]);
		dll_init(&(arg->philo[idx].logs));
		arg->philo[idx].arg = arg;
		arg->philo[idx].end = FALSE;
		arg->philo[idx].last_eat = 0;
		if (pthread_create(&(arg->philo[idx].thrd), T_NULL,
				philo_thread_func, &(arg->philo[idx])) != 0)
			return (main_thread_end(arg, idx, "pthread create error!"));
	}
	if (pthread_create(&(arg->philo[arg->philo_num].thrd), T_NULL,
			print_thread_func, arg) != 0)
		return (main_thread_end(arg, arg->philo_num,
				"pthread create error!"));
	if (pthread_create(&(arg->philo[arg->philo_num + 1].thrd), T_NULL,
			time_thread_func, arg) != 0)
		return (main_thread_end(arg, arg->philo_num + 1,
				"pthread create error!"));
	return (mutexes_init(arg));
}

inline static int	mutexes_init(t_arg *arg)
{
	while (++(arg->fork_cnt) < arg->philo_num)
	{
		arg->fork[arg->fork_cnt].status = UNUSE;
		if (pthread_mutex_init(&arg->fork[arg->fork_cnt].mtx,
				T_NULL) != 0)
			return (main_thread_end(arg, arg->philo_num + 2,
					"pthread mutex init error!"));
	}
	if (mtxs_init(arg->last_eat_mtx,
			arg->philo_num, &arg->last_eat_mtx_cnt) != 0
		|| mtxs_init(arg->log_mtx,
			arg->philo_num, &arg->log_mtx_cnt) != 0)
		return (main_thread_end(arg, arg->philo_num + 2,
				"pthread mutex init error!"));
	gettimeofday(&(arg->start), T_NULL);
	return (philosopher_end(arg));
}

inline static int	philosopher_end(t_arg *arg)
{
	int	idx;

	pthread_mutex_unlock(&(arg->start_flag));
	idx = -1;
	while (++idx < arg->philo_num)
		pthread_join(arg->philo[idx].thrd, T_NULL);
	if (chk_end(arg) == NORMAL)
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
	pthread_mutex_lock(&arg->end_flag_mtx);
	arg->end_flag = ABORT;
	pthread_mutex_unlock(&arg->end_flag_mtx);
	pthread_mutex_unlock(&arg->start_flag);
	arg_pthreads_join(arg, idx);
	philos_log_clear(arg, idx);
	arg_mutexes_destroy(arg);
	arg_free(arg);
	return (err_msg(arg, msg, 1));
}
