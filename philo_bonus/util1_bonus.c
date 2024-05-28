/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util1_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 16:12:58 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/27 16:08:59 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int	ft_atoi_int(const char *str)
{
	int	idx;
	int	sign;
	int	nb;

	idx = 0;
	sign = 1;
	nb = 0;
	while ((9 <= str[idx] && str[idx] <= 13) || str[idx] == ' ')
		idx++;
	if (str[idx] == '-' || str[idx] == '+')
		if (str[idx++] == '-')
			sign = -1;
	while ('0' <= str[idx] && str[idx] <= '9')
	{
		if (nb > INT_MAX / 10
			|| (nb == (INT_MAX / 10)
				&& str[idx] - '0' > (INT_MAX % 10) - ((sign - 1) / 2)))
			return (0);
		nb = nb * 10 + (str[idx++] - '0');
	}
	return ((int)nb * sign);
}

t_bool	ft_isdecimal(char *str)
{
	int	idx;

	idx = 0;
	if (str[0] == '-' || str[0] == '+')
		idx++;
	while (str[idx] != '\0')
	{
		if (!('0' <= str[idx] && str[idx] <= '9'))
			return (FALSE);
		idx++;
	}
	return (TRUE);
}

t_bool	ft_usleep(long us, t_timeval last_eat, int d_time)
{
	t_timeval	start;
	t_timeval	t;
	long		time_lapse;
	long		sleep_time;

	sleep_time = 10000;
	gettimeofday(&start, T_NULL);
	time_lapse = 0;
	while (time_lapse < us)
	{
		if (sleep_time >= 100)
			sleep_time /= 5;
		else
			sleep_time = 100;
		usleep(sleep_time);
		gettimeofday(&t, T_NULL);
		if (time_calc(t, last_eat) > d_time * MS_TO_US)
			return (FALSE);
		time_lapse = time_calc(t, start);
	}
	return (TRUE);
}

inline long	time_calc(t_timeval t1, t_timeval t2)
{
	return ((t1.tv_sec - t2.tv_sec) * S_TO_US
		+ (t1.tv_usec - t2.tv_usec));
}
