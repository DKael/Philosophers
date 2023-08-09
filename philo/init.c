/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 16:18:04 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/09 16:47:41 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	arg_init2(t_arg *arg, int argc, char **argv);

void	arg_init(t_arg *arg, int argc, char **argv)
{
	int	idx;

	idx = 0;
	while (++idx < argc)
	{
		if (ft_isdecimal(argv[idx]) == FALSE)
			err_msg("Invalid input! Input must be number.\n", 1);
		if (argv[idx][0] == '-')
			err_msg("Invalid input! Input must be positive.\n", 1);
	}
	arg->philo_num = ft_atoi_int(argv[1]);
	if (arg->philo_num == 0 && argv[1][0] == '0')
		err_msg("Invalid input! Wrong range of input value.\n", 1);
	arg->d_time = ft_atoi_int(argv[2]);
	if (arg->d_time == 0 && argv[1][0] == '0')
		err_msg("Invalid input! Wrong range of input value.\n", 1);
	arg->e_time = ft_atoi_int(argv[3]);
	if (arg->e_time == 0 && argv[1][0] == '0')
		err_msg("Invalid input! Wrong range of input value.\n", 1);
	arg->s_time = ft_atoi_int(argv[4]);
	if (arg->s_time == 0 && argv[1][0] == '0')
		err_msg("Invalid input! Wrong range of input value.\n", 1);
	arg_init2(arg, argc, argv);
}

static void	arg_init2(t_arg *arg, int argc, char **argv)
{
	arg->eat_cnt = -1;
	if (argc == 6)
	{
		arg->eat_cnt = ft_atoi_int(argv[5]);
		if (arg->eat_cnt == 0 && argv[1][0] == '0')
			err_msg("Invalid input! Wrong range of input value.\n", 1);
	}
	arg->s_flag = FALSE;
	arg->d_flag = 0;
	arg->philo = T_NULL;
	arg->fork = T_NULL;
}
