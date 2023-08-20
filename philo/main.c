/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 20:36:06 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/20 20:28:07 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int check_end_flag(t_arg *arg)
{
    int return_value;

    pthread_mutex_lock(&arg->end_flag_mtx);
    if (arg->end_flag != 0)
        return_value = 1;
    else
        return_value = 0;
    pthread_mutex_unlock(&arg->end_flag_mtx);
    return (return_value);
}

void arg_pthreads_join(t_arg *arg, int cnt)
{
    int idx;

    idx = -1;
    while (++idx < cnt)
        pthread_join(arg->philo[idx].thrd, NULL);
}

void arg_mutexes_destroy(t_arg *arg)
{
    int idx;

    if (arg->start_flag_chk == TRUE)
        pthread_mutex_destroy(&arg->start_flag);
    if (arg->end_flag_mtx_chk == TRUE)
        pthread_mutex_destroy(&arg->end_flag_mtx);
    idx = -1;
    while (++idx < arg->fork_cnt)
        pthread_mutex_destroy(&arg->fork[idx]);
    idx = -1;
    while (++idx < arg->last_eat_mtx_cnt)
        pthread_mutex_destroy(&arg->last_eat_mtx[idx]);
    idx = -1;
    while (++idx < arg->log_mtx_cnt)
        pthread_mutex_destroy(&arg->log_mtx[idx]);
}

int main_thread_end(t_arg *arg, int idx, const char *msg)
{
    pthread_mutex_lock(&arg->end_flag_mtx);
    arg->end_flag = TRUE;
    pthread_mutex_unlock(&arg->end_flag_mtx);
    pthread_mutex_unlock(&arg->start_flag);
    arg_pthreads_join(arg, idx);
    philos_log_clear(arg, idx);
    arg_mutexes_destroy(arg);
    arg_free(arg);
    return (err_msg(arg, msg, 1));
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
    if (arg_init(&arg, argc, argv) != 0)
        return (1);

    if (pthread_mutex_init(&arg.start_flag, NULL) != 0)
    {
        arg_free(&arg);
        return (1);
    }
    arg.start_flag_chk = TRUE;
    pthread_mutex_lock(&arg.start_flag);

    if (pthread_mutex_init(&arg.end_flag_mtx, NULL) != 0)
    {
        arg_mutexes_destroy(&arg);
        arg_free(&arg);
        return (1);
    }
    arg.end_flag_mtx_chk = TRUE;

    int idx;

    idx = -1;
    while (++idx < arg.philo_num)
    {
        arg.philo[idx].idx = idx;
        if (idx % 2 == 0)
        {
            arg.philo[idx].first_fork = &arg.fork[idx];
            arg.philo[idx].second_fork = &arg.fork[(idx + 1) % arg.philo_num];
        }
        else
        {
            arg.philo[idx].first_fork = &arg.fork[(idx + 1) % arg.philo_num];
            arg.philo[idx].second_fork = &arg.fork[idx];
        }
        dll_init(&arg.philo[idx].logs);
        arg.philo[idx].arg = &arg;
        if (pthread_create(&arg.philo[idx].thrd, NULL, philo_thread_func, &arg.philo[idx]) != 0)
            return (main_thread_end(&arg, idx, "pthread create error!"));
    }
    if (pthread_create(&arg.philo[arg.philo_num].thrd, NULL, print_thread_func, &arg) != 0)
        return (main_thread_end(&arg, arg.philo_num, "pthread create error!"));
    if (pthread_create(&arg.philo[arg.philo_num + 1].thrd, NULL, time_thread_func, &arg) != 0)
        return (main_thread_end(&arg, arg.philo_num + 1, "pthread create error!"));

    arg.fork_cnt = -1;
    while (++arg.fork_cnt < arg.philo_num)
        if (pthread_mutex_init(&arg.fork[arg.fork_cnt], NULL) != 0)
            return (main_thread_end(&arg, arg.philo_num + 2, "pthread mutex init error!"));

    arg.last_eat_mtx_cnt = -1;
    while (++arg.last_eat_mtx_cnt < arg.philo_num)
        if (pthread_mutex_init(&arg.last_eat_mtx[arg.last_eat_mtx_cnt], NULL) != 0)
            return (main_thread_end(&arg, arg.philo_num + 2, "pthread mutex init error!"));

    arg.log_mtx_cnt = -1;
    while (++arg.log_mtx_cnt < arg.philo_num)
        if (pthread_mutex_init(&arg.log_mtx[arg.log_mtx_cnt], NULL) != 0)
            return (main_thread_end(&arg, arg.philo_num + 2, "pthread mutex init error!"));

    if (gettimeofday(&arg.start, NULL) != 0)
        return (main_thread_end(&arg, arg.philo_num + 2, "gettimeofday error!"));

    pthread_mutex_unlock(&arg.start_flag);

    idx = -1;
    while (++idx < arg.philo_num + 2)
    {
        if (idx == arg.philo_num)
        {
            pthread_mutex_lock(&arg.end_flag_mtx);
            arg.end_flag = TRUE;
            pthread_mutex_unlock(&arg.end_flag_mtx);
        }
        pthread_join(arg.philo[idx].thrd, NULL);
    }
    arg_mutexes_destroy(&arg);
    arg_free(&arg);

    return (0);
}
