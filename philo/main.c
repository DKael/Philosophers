/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 20:36:06 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/13 18:06:03 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void *print_thread_func(void *arg)
{
}

void *time_thread_func(void *arg)
{
}

void *exit_thread(t_arg *arg, t_thread_status status)
{
    if (status == DEATH)
        arg->da_flag |= DEATH;
    else if (status == ABORT)
        arg->da_flag |= ABORT;
    return (T_NULL);
}

t_bool report(t_philo *value, t_philo_status status)
{
    t_log *log;
    t_dllnode *log_node;

    log = (t_log *)malloc(sizeof(t_log));
    if (log == T_NULL)
        return (FALSE);
    if (gettimeofday(&log->time, T_NULL) != 0)
    {
        free(log);
        return (FALSE);
    }
    log->who = value->idx + 1;
    log->status = status;
    log_node = dll_new_node(log);
    if (log_node == T_NULL)
    {
        free(log);
        return (FALSE);
    }
    dll_add_tail(&value->logs, log_node);
    return (TRUE);
}

void *philo_thread_func(void *param)
{
    t_philo *value;
    t_arg *arg;
    t_log *log;
    int eat_cnt;
    int cnt;

    value = (t_philo *)param;
    arg = (t_arg *)value->arg;
    eat_cnt = 0;
    while (arg->start_flag == FALSE)
    {
        if (usleep(10) != 0)
            return (exit_thread(arg, ABORT));
    }
    while (arg->da_flag == 0)
    {
        if (pthread_mutex_lock(value->first_fork) != 0)
            return (exit_thread(arg, ABORT));
        if (pthread_mutex_lock(value->second_fork) != 0)
            return (exit_thread(arg, ABORT));
        if (report(value, GET_FORK) == FALSE)
            return (exit_thread(arg, ABORT));
        if (report(value, EATING) == FALSE)
            return (exit_thread(arg, ABORT));
        cnt = -1;
        while (++cnt < arg->e_time)
            usleep(1000);
        if (pthread_mutex_unlock(value->first_fork) != 0)
            return (exit_thread(arg, ABORT));
        if (pthread_mutex_unlock(value->second_fork) != 0)
            return (exit_thread(arg, ABORT));
        if (gettimeofday(&value->last_eat, T_NULL) != 0)
            return (exit_thread(arg, ABORT));
        if (arg->have_to_eat != -1 && ++eat_cnt == arg->have_to_eat)
            return (T_NULL);
        if (report(value, SLEEPING) == FALSE)
            return (exit_thread(arg, ABORT));
        cnt = -1;
        while (++cnt < arg->s_time)
            usleep(1000);
        if (report(value, THINKING) == FALSE)
            return (exit_thread(arg, ABORT));
    }
    return (T_NULL);
}

int main(int argc, char **argv)
{
    t_arg arg;

    if (argc != 5 && argc != 6)
    {
        printf("Usage : %s number_of_philosophers \
        time_to_die time_to_eat time_to_sleep \
        [number_of_times_each_philosopher_must_eat]\n",
               argv[0]);
        return (1);
    }
    err_init(argv[0]);
    if (arg_init(&arg, argc, argv) != 0)
        return (1);
    arg.philo = (t_philo *)ft_calloc(arg.philo_num, sizeof(t_philo));
    if (arg.philo == T_NULL)
    {
        err_msg("malloc error!");
        return (1);
    }
    arg.fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * arg.philo_num);
    if (arg.fork == T_NULL)
    {
        free(arg.philo);
        err_msg("malloc error!");
        return (1);
    }

    int idx;

    idx = -1;
    while (++idx < arg.philo_num)
    {
        arg.philo[idx].idx = idx;
        if (idx % 2 == 0)
        {
            arg.philo[idx].first_fork = &arg.fork[idx];
            arg.philo[idx].second_fork = &arg.fork[idx + 1 % arg.philo_num];
        }
        else
        {
            arg.philo[idx].first_fork = &arg.fork[idx + 1 % arg.philo_num];
            arg.philo[idx].second_fork = &arg.fork[idx];
        }
        dll_init(&arg.philo[idx].logs);
        arg.philo[idx].arg = &arg;
        if (pthread_create(&arg.philo[idx].thrd, NULL, philo_thread_func, &arg.philo[idx]) != 0)
        {
            arg_free(&arg);
            err_msg("pthread create error!");
            return (1);
        }
        if (pthread_mutex_init(&arg.fork[idx], NULL) != 0)
        {
            arg_free(&arg);
            err_msg("pthread mutex init error!");
            return (1);
        }
    }
    if (pthread_create(&arg.print_thrd, NULL, print_thread_func, &arg) != 0)
    {
        arg_free(&arg);
        err_msg("pthread create error!");
        return (1);
    }
    if (pthread_create(&arg.time_thrd, NULL, time_thread_func, &arg) != 0)
    {
        arg_free(&arg);
        err_msg("pthread create error!");
        return (1);
    }
    gettimeofday(&arg.start, T_NULL);
    arg.start_flag = TRUE;

    idx = -1;
    while (++idx < arg.philo_num)
    {
        if (pthread_join(arg.philo[idx].thrd, NULL) != 0)
        {
            arg_free(&arg);
            err_msg("pthread join error\n");
            return (1);
        }
        if (pthread_mutex_destroy(&arg.fork[idx]) != 0)
        {
            arg_free(&arg);
            err_msg("pthread mutex destroy error\n");
            return (1);
        }
    }
    arg_free(&arg);
    return 0;
}
