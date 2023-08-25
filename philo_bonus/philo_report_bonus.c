/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_report_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 16:10:17 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/25 13:32:48 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static t_bool	report2(t_philo *value, t_log *log, t_arg *arg);

t_bool	report(t_philo *value, t_philo_status status, t_arg *arg)
{
	t_log	*log;

	log = (t_log *)malloc(sizeof(t_log));
	if (log == T_NULL)
		return (FALSE);
	if (gettimeofday(&log->time, T_NULL) != 0)
	{
		free(log);
		return (FALSE);
	}
	log->usec = (log->time.tv_sec - arg->start.tv_sec) * S_TO_US
		+ (log->time.tv_usec - arg->start.tv_usec);
	if (status == EATING)
	{
		sem_wait_nointr(arg->last_eat_sem[value->idx].sem);
		value->last_eat = log->usec;
		sem_post(arg->last_eat_sem[value->idx].sem);
	}
	log->who = value->idx + 1;
	log->status = status;
	return (report2(value, log, arg));
}

static t_bool	report2(t_philo *value, t_log *log, t_arg *arg)
{
	t_dllnode	*log_node;

	log_node = dll_new_node((void *)log);
	if (log_node == T_NULL)
	{
		free(log);
		return (FALSE);
	}
	sem_wait_nointr(arg->log_sem[value->idx].sem);
	dll_add_tail(&value->logs, log_node);
	sem_post(arg->log_sem[value->idx].sem);
	return (TRUE);
}

t_bool	die_report(t_arg *arg, long time_lapse_usec, int idx)
{
	t_log		*log;
	t_dllnode	*log_node;

	log = (t_log *)malloc(sizeof(t_log));
	if (log == T_NULL)
		return (FALSE);
	log->usec = time_lapse_usec;
	log->who = idx + 1;
	log->status = DIE;
	log_node = dll_new_node((void *)log);
	if (log_node == T_NULL)
	{
		free(log);
		return (FALSE);
	}
	sem_wait_nointr(arg->log_sem[idx].sem);
	dll_add_tail(&arg->philo[idx].logs, log_node);
	sem_post(arg->log_sem[idx].sem);
	return (TRUE);
}
