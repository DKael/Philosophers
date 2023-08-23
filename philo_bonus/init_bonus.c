/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 16:18:04 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/23 15:14:21 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static int	arg_init2(t_arg *arg, int argc, char **argv);
static int	arg_init3(t_arg *arg);

int	arg_init(t_arg *arg, int argc, char **argv)
{
	int	idx;

	arg->program_name = argv[0];
	idx = 0;
	while (++idx < argc)
	{
		if (ft_isdecimal(argv[idx]) == FALSE)
			return (err_msg(arg, "Invalid input! Input must be number.", 1));
		if (argv[idx][0] == '-')
			return (err_msg(arg, "Invalid input! Input must be positive.", 1));
	}
	arg->philo_num = ft_atoi_int(argv[1]);
	if (arg->philo_num == 0 && argv[1][0] != '0')
		return (err_msg(arg, "Invalid input! Wrong range of input value.", 1));
	arg->d_time = ft_atoi_int(argv[2]);
	if (arg->d_time == 0 && argv[2][0] != '0')
		return (err_msg(arg, "Invalid input! Wrong range of input value.", 1));
	arg->e_time = ft_atoi_int(argv[3]);
	if (arg->e_time == 0 && argv[3][0] != '0')
		return (err_msg(arg, "Invalid input! Wrong range of input value.", 1));
	arg->s_time = ft_atoi_int(argv[4]);
	if (arg->s_time == 0 && argv[4][0] != '0')
		return (err_msg(arg, "Invalid input! Wrong range of input value.", 1));
	return (arg_init2(arg, argc, argv));
}

static int	arg_init2(t_arg *arg, int argc, char **argv)
{
	arg->have_to_eat = -1;
	if (argc == 6)
	{
		arg->have_to_eat = ft_atoi_int(argv[5]);
		if (arg->have_to_eat == 0 && argv[5][0] != '0')
			return (err_msg(arg,
					"Invalid input! Wrong range of input value.", 1));
	}
	arg->philo = T_NULL;
	arg->fork_chk = FALSE;
	arg->last_eat_sem = T_NULL;
	arg->last_eat_sem_cnt = -1;
	arg->log_sem = T_NULL;
	arg->log_sem_cnt = -1;
	arg->start_flag_chk = FALSE;
	arg->end_flag_sem_chk = FALSE;
	arg->end_flag = NORMAL;
	return (arg_init3(arg));
}

static int	arg_init3(t_arg *arg)
{
	arg->philo = (t_philo *)ft_calloc(arg->philo_num, sizeof(t_philo));
	if (arg->philo == T_NULL)
		return (err_msg(arg, "malloc error!", 1));
	arg->last_eat_sem = (sem_t **)malloc(sizeof(sem_t *)
			* arg->philo_num);
	if (arg->last_eat_sem == T_NULL)
	{
		arg_free(arg);
		return (err_msg(arg, "malloc error!", 1));
	}
	arg->log_sem = (sem_t **)malloc(sizeof(sem_t *)
			* arg->philo_num);
	if (arg->log_sem == T_NULL)
	{
		arg_free(arg);
		return (err_msg(arg, "malloc error!", 1));
	}
	return (0);
}

int	arg_sems_init(sem_t **lst, int num, int *sem_cnt)
{
	*sem_cnt = -1;
	while (++(*sem_cnt) < num)
		if (sem_open(&lst[*sem_cnt], T_NULL) != 0)
			return (1);
	return (0);
}
