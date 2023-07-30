//* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:12:01 by hyungdki          #+#    #+#             */
/*   Updated: 2023/07/30 23:10:30 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	arg_init(t_philo *data, int argc, char **argv, int idx)
{
	while (++idx < argc)
	{
		if (argv[idx][0] == '-')
			err_msg("Invalid input! Input must be positive.\n", 1);
	}
	data->philo_num = ft_atoi_int(argv[1]);
	if (data->philo_num == 0 && argv[1][0] == '0')
		err_msg("Invalid input! Wrong range of input value.\n", 1);
	data->d_time = ft_atoi_int(argv[2]);
	if (data->d_time == 0 && argv[1][0] == '0')
		err_msg("Invalid input! Wrong range of input value.\n", 1);
	data->e_time = ft_atoi_int(argv[3]);
	if (data->e_time == 0 && argv[1][0] == '0')
		err_msg("Invalid input! Wrong range of input value.\n", 1);
	data->s_time = ft_atoi_int(argv[4]);
	if (data->s_time == 0 && argv[1][0] == '0')
		err_msg("Invalid input! Wrong range of input value.\n", 1);
	if (argc == 6)
	{
		data->eat_cnt = ft_atoi_int(argv[5]);
		if (data->eat_cnt == 0 && argv[1][0] == '0')
			err_msg("Invalid input! Wrong range of input value.\n", 1);
	}
}

void	philo_init(t_philo *data)
{
	data->philo_num = -1;
	data->d_time = -1;
	data->e_time = -1;
	data->s_time = -1;
	data->eat_cnt = -1;
	data->philo = T_NULL;
	data->fork = T_NULL;
}

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
;