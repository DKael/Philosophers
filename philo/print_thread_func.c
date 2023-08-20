/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_thread_func.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:58:56 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/20 20:30:14 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int srt_time_compare(void *input_lst, int idx1, int idx2)
{
	t_srt *lst;

	lst = (t_srt *)input_lst;
	if (lst[idx1].usec > lst[idx2].usec)
		return (1);
	else if (lst[idx1].usec < lst[idx2].usec)
		return (-1);
	else
	{
		if (lst[idx1].status > lst[idx2].status)
			return (1);
		else if (lst[idx1].status < lst[idx2].status)
			return (-1);
		else
			return (0);
	}
}

void srt_swap(void *input_lst, int idx1, int idx2)
{
	t_srt *lst;
	t_srt temp;

	lst = (t_srt *)input_lst;
	temp = lst[idx1];
	lst[idx1] = lst[idx2];
	lst[idx2] = temp;
}

void *print_thread_func(void *input_arg)
{
	t_timeval time_lapse;
	long time_lapse_usec;
	t_dll total_logs;
	t_dllnode *temp_node;
	t_dllnode *temp_dll;
	t_log *temp_log;
	t_arg *arg;
	t_srt *srt;
	int idx;
	long time_offset;

	arg = (t_arg *)input_arg;
	dll_init(&total_logs);
	if (arg->philo_num < 100)
		time_offset = 1000;
	else if (arg->philo_num >= 500)
		time_offset = 5000;
	else
		time_offset = arg->philo_num * 10;
	pthread_mutex_lock(&arg->start_flag);
	pthread_mutex_unlock(&arg->start_flag);
	while (check_end_flag(arg) == 0)
	{
		if (usleep(1000) == EINTR)
			printf("Interrupted by a signa\n");
		if (gettimeofday(&time_lapse, NULL) != 0)
			return (thread_error_end(arg));
		time_lapse_usec = (time_lapse.tv_sec - arg->start.tv_sec) * S_TO_US + (time_lapse.tv_usec - arg->start.tv_usec);
		idx = -1;
		while (++idx < arg->philo_num)
		{
			temp_dll = &(arg->philo[idx].logs.head);
			pthread_mutex_lock(&arg->log_mtx[idx]);
			while (arg->philo[idx].logs.size != 0 && ((t_log *)temp_dll->back->contents)->usec < time_lapse_usec - time_offset)
			{
				temp_node = temp_dll->back;
				temp_dll->back->back->front = temp_dll;
				temp_dll->back = temp_dll->back->back;
				arg->philo[idx].logs.size--;
				dll_add_tail(&total_logs, temp_node);
				temp_node = NULL;
			}
			pthread_mutex_unlock(&arg->log_mtx[idx]);
		}
		if (total_logs.size == 0)
			continue;
		srt = (t_srt *)malloc(sizeof(t_srt) * total_logs.size);
		if (srt == NULL)
		{
			dll_clear(&total_logs, log_delete_func);
			return (thread_error_end(arg));
		}
		idx = -1;
		temp_dll = total_logs.head.back;
		while (++idx < total_logs.size)
		{
			srt[idx].usec = ((t_log *)(temp_dll->contents))->usec;
			srt[idx].status = ((t_log *)(temp_dll->contents))->status;
			srt[idx].ptr = temp_dll;
			temp_dll = temp_dll->back;
		}
		sorting((void *)srt, total_logs.size, srt_time_compare, srt_swap);
		idx = -1;
		while (++idx < total_logs.size)
		{
			temp_log = (t_log *)(srt[idx].ptr->contents);
			if (temp_log->status == GET_FORK)
				printf("%ld %d has taken a fork\n", temp_log->usec / 1000, temp_log->who);
			else if (temp_log->status == EATING)
				printf("%ld %d is eating\n", temp_log->usec / 1000, temp_log->who);
			else if (temp_log->status == SLEEPING)
				printf("%ld %d is sleeping\n", temp_log->usec / 1000, temp_log->who);
			else if (temp_log->status == THINKING)
				printf("%ld %d is thinking\n", temp_log->usec / 1000, temp_log->who);
			else if (temp_log->status == DIE)
			{
				printf("%ld %d is died\n", temp_log->usec / 1000, temp_log->who);
				dll_clear(&total_logs, log_delete_func);
				free(srt);
				pthread_mutex_lock(&arg->end_flag_mtx);
				arg->end_flag = TRUE;
				pthread_mutex_unlock(&arg->end_flag_mtx);
				return (NULL);
			}
		}
		dll_clear(&total_logs, log_delete_func);
		free(srt);
	}
	return (NULL);
}
