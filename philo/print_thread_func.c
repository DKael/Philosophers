/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_thread_func.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:58:56 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/15 17:55:47 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	srt_time_compare(void *input_lst, int idx1, int idx2)
{
	t_srt	*lst;

	lst = (t_srt *)input_lst;
	if (lst[idx1].time > lst[idx2].time)
		return (1);
	else if (lst[idx1].time < lst[idx2].time)
		return (-1);
	else
		return (0);
}

void	srt_swap(void *input_lst, int idx1, int idx2)
{
	t_srt	*lst;
	t_srt	temp;

	lst = (t_srt *)input_lst;
	temp = lst[idx1];
	lst[idx1] = lst[idx2];
	lst[idx2] = temp;
}

void *print_thread_func(void *input_arg)
{
    t_timeval	time_lapse;
    t_dll		total_logs;
	t_dllnode	*temp_node;
	t_dllnode	*temp_dll;
	t_log		*temp_log;
	int			temp_time;
    t_arg		*arg;
	t_srt		*srt;
	int			idx;
	int			time_offset;

    arg = (t_arg *)input_arg;
    dll_init(&total_logs);
	if (arg->philo_num < 100)
		time_offset = 1000;
	else
		time_offset = (arg->philo_num / 10) * 100;
    while (arg->start_flag == FALSE)
    {
        if (usleep(10) != 0)
            printf("usleep function is interrupted by a signal\n");
    }
    while (1)
    {
        if (gettimeofday(&time_lapse, T_NULL) != 0)
            return (exit_thread(arg, ABORT, T_NULL));
        idx = -1;
		while (++idx < arg->philo_num)
		{
			temp_dll = &(arg->philo[idx].logs.head);
			while (arg->philo[idx].logs.size != 0
				&& ((t_log *)temp_dll->back->contents)->time.tv_usec < time_lapse.tv_usec - time_offset)
			{
				temp_node = temp_dll->back;
				temp_dll->back->back->front = temp_dll;
				temp_dll->back = temp_dll->back->back;
				arg->philo[idx].logs.size--;
				dll_add_tail(&total_logs, temp_node);
				temp_node = T_NULL;
			}
		}
		if (total_logs.size == 0)
			continue;
		srt = (t_srt *)malloc(sizeof(t_srt) * total_logs.size);
		if (srt == T_NULL)
			return (exit_thread(arg, ABORT, &total_logs));
		idx = -1;
		temp_dll = total_logs.head.back;
		while (++idx < total_logs.size)
		{
			srt[idx].time = ((t_log *)(temp_dll->contents))->time.tv_usec;
			srt[idx].ptr = temp_dll;
			temp_dll = temp_dll->back;
		}
		sorting((void *)srt, total_logs.size, srt_time_compare, srt_swap);
		idx = -1;
		while (++idx < total_logs.size)
		{
			temp_log = (t_log *)(srt[idx].ptr->contents);
			temp_time = temp_log->time.tv_usec - arg->start.tv_usec;
			if (temp_log->status == GET_FORK)
				printf("%d %d has taken a fork\n", temp_time, temp_log->who);
			else if (temp_log->status == EATING)
				printf("%d %d is eating\n", temp_time, temp_log->who);
			else if (temp_log->status == SLEEPING)
				printf("%d %d is sleeping\n", temp_time, temp_log->who);
			else if (temp_log->status == THINKING)
				printf("%d %d is thinking\n", temp_time, temp_log->who);
		}
		dll_clear(&total_logs, log_delete_func);
		free(srt);
    }
    return (T_NULL);
}
