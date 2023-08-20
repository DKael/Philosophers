/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:06:11 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/20 19:54:42 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int err_msg(t_arg *arg, const char *msg, int return_code)
{
	if (msg != NULL)
		printf("%s: %s\n", arg->program_name, msg);
	else
		printf("%s: undefined error\n", arg->program_name);
	return (return_code);
}

void	arg_free(t_arg *arg)
{
	free(arg->philo);
	free(arg->fork);
	free(arg->last_eat_mtx);
	free(arg->log_mtx);
}

void	philos_log_clear(t_arg *arg, int cnt)
{
	int idx;

    idx = -1;
    while (++idx < cnt)
        dll_clear(&arg->philo[idx].logs, log_delete_func);
}

void log_delete_func(void *content)
{
	t_log *temp;

	if (content != NULL)
	{
		temp = (t_log *)content;
		free(temp);
	}
}

void	*thread_error_end(t_arg *arg)
{
	pthread_mutex_lock(&arg->end_flag_mtx);
	arg->end_flag = TRUE;
	pthread_mutex_unlock(&arg->end_flag_mtx);
	return (NULL);
}