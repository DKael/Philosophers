/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resource_free_func.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 19:33:08 by hyungdki          #+#    #+#             */
/*   Updated: 2024/01/27 16:21:50 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	arg_free(t_arg *arg)
{
	free(arg->philo);
	free(arg->fork);
	free(arg->last_eat_mtx);
	free(arg->log_mtx);
}

void	log_delete_func(void *log)
{
	t_log	*temp;

	if (log != T_NULL)
	{
		temp = (t_log *)log;
		free(temp);
	}
}

void	philos_log_clear(t_arg *arg, int cnt)
{
	int	idx;

	idx = -1;
	while (++idx < cnt)
		dll_clear(&arg->philo[idx].logs, log_delete_func);
}

void	arg_pthreads_join(t_arg *arg, int cnt)
{
	int	idx;

	idx = -1;
	while (++idx < cnt)
		pthread_join(arg->philo[idx].thrd, T_NULL);
}

void	arg_mutexes_destroy(t_arg *arg)
{
	int	idx;

	if (arg->start_flag_chk == TRUE)
		pthread_mutex_destroy(&arg->start_flag);
	if (arg->end_flag_mtx_chk == TRUE)
		pthread_mutex_destroy(&arg->end_flag_mtx);
	idx = -1;
	while (++idx < arg->fork_cnt)
		pthread_mutex_destroy(&arg->fork[idx].mtx);
	idx = -1;
	while (++idx < arg->last_eat_mtx_cnt)
		pthread_mutex_destroy(&arg->last_eat_mtx[idx]);
	idx = -1;
	while (++idx < arg->log_mtx_cnt)
		pthread_mutex_destroy(&arg->log_mtx[idx]);
}
