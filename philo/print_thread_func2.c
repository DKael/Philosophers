/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_thread_func2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 17:42:48 by hyungdki          #+#    #+#             */
/*   Updated: 2024/01/27 17:04:41 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void		log_collect(t_arg *arg, t_dll *total_logs,
					long time_offset, t_timeval t_lapse);
static t_srt	*srt_log_sorting(t_dll *total_logs);
static int		log_print(t_arg *arg, t_dll *total_logs,
					t_srt *srt, int *eat_done_cnt);
static int		log_print_case_die(t_arg *arg, t_dll *total_logs,
					t_log *temp_log, t_srt *srt);
extern int		srt_time_compare(void *input_lst, int idx1, int idx2);
extern void		srt_swap(void *input_lst, int idx1, int idx2);

int	print_thread_func2(t_arg *arg, long time_offset)
{
	int			eat_done_cnt;
	t_dll		total_logs;
	t_srt		*srt;
	t_timeval	t_lapse;

	dll_init(&total_logs);
	eat_done_cnt = 0;
	while (chk_end(arg) != ABORT && eat_done_cnt < arg->philo_num)
	{
		gettimeofday(&t_lapse, T_NULL);
		log_collect(arg, &total_logs, time_offset, t_lapse);
		if (total_logs.size == 0)
			continue ;
		srt = srt_log_sorting(&total_logs);
		if (srt == T_NULL)
			return (1);
		if (log_print(arg, &total_logs, srt, &eat_done_cnt) == 1)
			return (0);
		dll_clear(&total_logs, log_delete_func);
		free(srt);
	}
	return (0);
}

inline static void	log_collect(t_arg *arg, t_dll *total_logs,
		long t_offset, t_timeval t_lapse)
{
	int			idx;
	long		usec;
	t_dllnode	*temp_dll;

	usec = time_calc(t_lapse, arg->start);
	idx = -1;
	while (++idx < arg->philo_num)
	{
		temp_dll = &(arg->philo[idx].logs.head);
		pthread_mutex_lock(&arg->log_mtx[idx]);
		while (temp_dll->back != &(arg->philo[idx].logs.tail)
			&& ((t_log *)temp_dll->back->contents)->usec < usec - t_offset)
			dll_node_move_to_another_dll_tail(temp_dll->back,
				&arg->philo[idx].logs, total_logs);
		pthread_mutex_unlock(&arg->log_mtx[idx]);
	}
}

inline static t_srt	*srt_log_sorting(t_dll *total_logs)
{
	t_srt		*srt;
	int			idx;
	t_dllnode	*temp_dll;

	srt = (t_srt *)malloc(sizeof(t_srt) * total_logs->size);
	if (srt == T_NULL)
	{
		dll_clear(total_logs, log_delete_func);
		return (T_NULL);
	}
	idx = -1;
	temp_dll = total_logs->head.back;
	while (++idx < total_logs->size)
	{
		srt[idx].usec = ((t_log *)(temp_dll->contents))->usec;
		srt[idx].status = ((t_log *)(temp_dll->contents))->status;
		srt[idx].ptr = temp_dll;
		temp_dll = temp_dll->back;
	}
	sorting((void *)srt, total_logs->size, srt_time_compare, srt_swap);
	return (srt);
}

inline static int	log_print(t_arg *arg, t_dll *total_logs,
				t_srt *srt, int *eat_done_cnt)
{
	int		idx;
	t_log	*temp_log;

	idx = -1;
	while (++idx < total_logs->size)
	{
		temp_log = (t_log *)(srt[idx].ptr->contents);
		if (temp_log->status == GET_FORK)
			printf("%ld %d has taken a fork\n",
				temp_log->usec / 1000, temp_log->who);
		else if (temp_log->status == EATING)
			printf("%ld %d is eating\n", temp_log->usec / 1000, temp_log->who);
		else if (temp_log->status == SLEEPING)
			printf("%ld %d is sleeping\n", temp_log->usec / 1000, temp_log->who);
		else if (temp_log->status == THINKING)
			printf("%ld %d is thinking\n", temp_log->usec / 1000, temp_log->who);
		else if (temp_log->status == DIE)
			return (log_print_case_die(arg, total_logs, temp_log, srt));
		else if (temp_log->status == EAT_DONE)
			(*eat_done_cnt)++;
	}
	return (0);
}

inline static int	log_print_case_die(t_arg *arg, t_dll *total_logs,
		t_log *temp_log, t_srt *srt)
{
	printf("%ld %d died\n", temp_log->usec / 1000, temp_log->who);
	dll_clear(total_logs, log_delete_func);
	free(srt);
	pthread_mutex_lock(&arg->end_flag_mtx);
	arg->end_flag = PHILO_DIE;
	pthread_mutex_unlock(&arg->end_flag_mtx);
	return (1);
}
