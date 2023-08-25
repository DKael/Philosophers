/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_thread_func1_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:58:56 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/25 13:33:00 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

extern int	print_thread_func2(t_arg *arg, long time_offset);

void	*print_thread_func(void *input_arg)
{
	t_arg	*arg;
	long	time_offset;

	arg = (t_arg *)input_arg;
	if (arg->philo_num < 100)
		time_offset = 500;
	else if (arg->philo_num >= 200)
		time_offset = 1000;
	else
		time_offset = arg->philo_num * 5;
	sem_wait_nointr(arg->start_flag.sem);
	sem_post(arg->start_flag.sem);
	if (usleep(time_offset) == EINTR)
		printf("Interrupted by a signa\n");
	if (print_thread_func2(arg, time_offset) == 1)
		return (thread_error_end(arg));
	return (T_NULL);
}
