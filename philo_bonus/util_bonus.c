/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 16:12:58 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/23 09:54:04 by hyungdki         ###   ########.fr       */
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

void	*ft_calloc(size_t count, size_t size)
{
	size_t	index;
	size_t	total_size;
	void	*result;
	char	*temp_ptr;

	index = 0;
	if (count == 0 || size == 0)
		return (malloc(0));
	total_size = count * size;
	if (ULLONG_MAX / size < count)
		return ((T_NULL));
	result = malloc(total_size);
	if (result == T_NULL)
		return (T_NULL);
	temp_ptr = (char *)result;
	while (index < total_size)
	{
		temp_ptr[index] = 0;
		index++;
	}
	return (result);
}

t_bool	ft_usleep(long us)
{
	t_timeval	start;
	t_timeval	t;
	long		time_lapse;
	long		sleep_time;

	if (gettimeofday(&start, T_NULL) != 0 || gettimeofday(&t, T_NULL) != 0)
		return (FALSE);
	time_lapse = (t.tv_sec - start.tv_sec) * S_TO_US
		+ (t.tv_usec - start.tv_usec);
	sleep_time = us;
	while (time_lapse < us)
	{
		if (sleep_time >= 20)
			sleep_time /= 5;
		else
			sleep_time = 10;
		if (usleep(sleep_time) != 0)
			printf("usleep function is interrupted by a signal\n");
		if (gettimeofday(&t, T_NULL) != 0)
			return (FALSE);
		time_lapse = (t.tv_sec - start.tv_sec) * S_TO_US
			+ (t.tv_usec - start.tv_usec);
	}
	return (TRUE);
}
