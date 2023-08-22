/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resource_free_func.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 19:33:08 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/21 20:14:30 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philosophers.h"

void	arg_free(t_arg *arg)
{
	free(arg->philo);
	free(arg->fork);
	free(arg->last_eat_mtx);
	free(arg->log_mtx);
}

void log_delete_func(void *log)
{
	t_log *temp;

	if (log != T_NULL)
	{
		temp = (t_log *)log;
		free(temp);
	}
}

void	philos_log_clear(t_arg *arg, int cnt)
{
	int idx;

    idx = -1;
    while (++idx < cnt)
        dll_clear(&arg->philo[idx].logs, log_delete_func);
}

void arg_pthreads_join(t_arg *arg, int cnt)
{
    int idx;

    idx = -1;
    while (++idx < cnt)
        pthread_join(arg->philo[idx].thrd, T_NULL);
}



void arg_mutexes_destroy(t_arg *arg)
{
    int idx;

    if (arg->start_flag_chk == TRUE)
        pthread_mutex_destroy(&arg->start_flag);
    if (arg->end_flag_mtx_chk == TRUE)
        pthread_mutex_destroy(&arg->end_flag_mtx);
    idx = -1;
    while (++idx < arg->fork_cnt)
        pthread_mutex_destroy(&arg->fork[idx]);
    idx = -1;
    while (++idx < arg->last_eat_mtx_cnt)
        pthread_mutex_destroy(&arg->last_eat_mtx[idx]);
    idx = -1;
    while (++idx < arg->log_mtx_cnt)
        pthread_mutex_destroy(&arg->log_mtx[idx]);
}

int	check_end_flag(t_arg *arg)
{
	int	return_value;

	pthread_mutex_lock(&arg->end_flag_mtx);
	if (arg->end_flag != 0)
		return_value = 1;
	else
		return_value = 0;
	pthread_mutex_unlock(&arg->end_flag_mtx);
	return (return_value);
}

int	arg_mutexes_init(t_arg *arg, int *mtx_cnt)
{
	*mtx_cnt = -1;
	while (++(*mtx_cnt) < arg->philo_num)
		if (pthread_mutex_init(&arg->fork[*mtx_cnt], T_NULL) != 0)
			return (1);
	return (0);
}

int	main_thread_end(t_arg *arg, int idx, const char *msg)
{
	pthread_mutex_lock(&arg->end_flag_mtx);
	arg->end_flag = TRUE;
	pthread_mutex_unlock(&arg->end_flag_mtx);
	pthread_mutex_unlock(&arg->start_flag);
	arg_pthreads_join(arg, idx);
	philos_log_clear(arg, idx);
	arg_mutexes_destroy(arg);
	arg_free(arg);
	return (err_msg(arg, msg, 1));
}