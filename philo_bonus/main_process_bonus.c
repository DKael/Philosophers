/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_process_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 16:17:11 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/27 16:15:48 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static int		make_philo_process(t_arg *arg);
static int		philosopher_end(t_arg *arg);
static int		main_process_err(t_arg *arg, int idx, const char *msg);

int	philosopher_start(int argc, char **argv)
{
	t_arg	arg;

	if (arg_init(&arg, argc, argv) != 0)
		return (1);
	arg.start_flag.name = "start_flag";
	arg.start_flag.sem = ft_sem_open("start_flag", 0644, 1);
	if (arg.start_flag.sem == SEM_FAILED)
		return (main_process_err(&arg, 0, "sem open failed!"));
	arg.start_flag_chk = TRUE;
	arg.fork.name = "fork";
	arg.fork.sem = ft_sem_open("fork", 0644, arg.philo_num);
	if (arg.fork.sem == SEM_FAILED)
		return (main_process_err(&arg, 0, "sem open failed!"));
	arg.fork_chk = TRUE;
	arg.print_sem.name = "print_sem";
	arg.print_sem.sem = ft_sem_open("print_sem", 0644, 1);
	if (arg.fork.sem == SEM_FAILED)
		return (main_process_err(&arg, 0, "sem open failed!"));
	arg.print_sem_chk = TRUE;
	if (make_multiple_sem(arg.last_eat_sem, "last_eat_sem",
			&arg.last_eat_sem_cnt, arg.philo_num) == 1)
		return (main_process_err(&arg, 0, "sem open failed!"));
	return (make_philo_process(&arg));
}

static int	make_philo_process(t_arg *arg)
{
	int	idx;

	sem_wait_nointr(arg->start_flag.sem);
	if (gettimeofday(&(arg->start), T_NULL) != 0)
		return (main_process_err(arg, 0, "gettimeofday failed!"));
	idx = -1;
	while (++idx < arg->philo_num)
	{
		arg->pid_lst[idx] = fork();
		if (arg->pid_lst[idx] == -1)
			return (main_process_err(arg, idx, "fork() error!"));
		else if (arg->pid_lst[idx] == 0)
			philo_process_func(arg, idx);
	}
	return (philosopher_end(arg));
}

static int	philosopher_end(t_arg *arg)
{
	int	idx;
	int	idx1;
	int	status;

	sem_post(arg->start_flag.sem);
	idx = -1;
	while (++idx < arg->philo_num)
	{
		waitpid(-1, &status, 0);
		if (WIFEXITED(status) != 0)
			break ;
	}
	if (WIFEXITED(status) != 0)
	{
		idx1 = -1;
		while (++idx1 < arg->philo_num)
			kill(arg->pid_lst[idx1], SIGKILL);
		while (++idx < arg->philo_num)
			waitpid(-1, T_NULL, 0);
	}
	arg_sems_destroy(arg);
	free(arg->pid_lst);
	return (0);
}

static int	main_process_err(t_arg *arg, int idx, const char *msg)
{
	kill_and_waitpid(arg, idx);
	arg_sems_destroy(arg);
	arg_heap_free(arg);
	return (err_msg(arg, msg, 1));
}
