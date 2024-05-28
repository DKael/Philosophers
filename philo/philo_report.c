/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_report.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 16:10:17 by hyungdki          #+#    #+#             */
/*   Updated: 2024/01/27 17:04:19 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static t_bool	report2(t_philo *value, t_log *log, t_arg *arg);

t_bool	report(t_philo *value, t_philo_status status, t_arg *arg)
{
	t_log	*log;

	log = (t_log *)malloc(sizeof(t_log));
	if (log == T_NULL)
		return (FALSE);
	gettimeofday(&log->time, T_NULL);
	log->usec = time_calc(log->time, arg->start);
	if (status == EATING)
	{
		pthread_mutex_lock(&arg->last_eat_mtx[value->idx]);
		value->last_eat = log->usec;
		pthread_mutex_unlock(&arg->last_eat_mtx[value->idx]);
	}
	log->who = value->idx + 1;
	log->status = status;
	return (report2(value, log, arg));
}

inline static t_bool	report2(t_philo *value, t_log *log, t_arg *arg)
{
	t_dllnode	*log_node;

	log_node = dll_new_node((void *)log);
	if (log_node == T_NULL)
	{
		free(log);
		return (FALSE);
	}
	pthread_mutex_lock(&arg->log_mtx[value->idx]);
	dll_add_tail(&value->logs, log_node);
	pthread_mutex_unlock(&arg->log_mtx[value->idx]);
	return (TRUE);
}
