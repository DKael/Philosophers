/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 20:36:06 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/15 22:26:46 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
        return (err_msg("malloc error!", 1));
    arg.fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * arg.philo_num);
    if (arg.fork == T_NULL)
        return (err_msg("malloc error!", 1));

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
        {
            arg.da_flag |= ABORT;
            while (++arg.end_flag < idx)
                ;
            arg_free(&arg);
            return (err_msg("pthread create error!", 1));
        }
        if (pthread_detach(arg.philo[idx].thrd) != 0)
        {
            arg.da_flag |= ABORT;
            pthread_join(arg.philo[idx].thrd, NULL);
            while (++arg.end_flag <= idx)
                ;
            arg_free(&arg);
            return (err_msg("pthread detach error!", 1));
        }
    }
    if (pthread_create(&arg.print_thrd, NULL, print_thread_func, &arg) != 0)
    {
        arg.da_flag |= ABORT;
        while (++arg.end_flag < arg.philo_num)
            ;
        arg_free(&arg);
        return (err_msg("pthread create error!", 1));
    }
    if (pthread_detach(arg.print_thrd) != 0)
    {
        arg.da_flag |= ABORT;
        pthread_join(arg.print_thrd, NULL);
        while (++arg.end_flag <= arg.philo_num)
            ;
        arg_free(&arg);
        return (err_msg("pthread detach error!", 1));
    }
    if (pthread_create(&arg.time_thrd, NULL, time_thread_func, &arg) != 0)
    {
        arg.da_flag |= ABORT;
        while (++arg.end_flag < arg.philo_num + 1)
            ;
        arg_free(&arg);
        return (err_msg("pthread create error!", 1));
    }
    if (pthread_detach(arg.time_thrd) != 0)
    {
        arg.da_flag |= ABORT;
        pthread_join(arg.time_thrd, NULL);
        while (++arg.end_flag <= arg.philo_num + 1)
            ;
        arg_free(&arg);
        return (err_msg("pthread detach error!", 1));
    }

    idx = -1;
    while (++idx < arg.philo_num)
    {
        if (pthread_mutex_init(&arg.fork[idx], NULL) != 0)
        {
            arg.end_flag = -1;
            while (++arg.end_flag < idx)
                if (pthread_mutex_destroy(&arg.fork[arg.end_flag]) == EINVAL)
                    printf("The value specified by mutex is invalid.\n");
            arg.da_flag |= ABORT;
            arg.end_flag = 0;
            while (++arg.end_flag < arg.philo_num + 2)
                ;
            arg_free(&arg);
            return (err_msg("pthread mutex init error!", 1));
        }
    }

    if (gettimeofday(&arg.start, T_NULL) != 0)
    {
        arg.end_flag = -1;
        while (++arg.end_flag < arg.philo_num)
            if (pthread_mutex_destroy(&arg.fork[arg.end_flag]) == EINVAL)
                printf("The value specified by mutex is invalid.\n");
        arg.da_flag |= ABORT;
        arg.end_flag = 0;
        while (++arg.end_flag < arg.philo_num + 2)
            ;
        arg_free(&arg);
        return (err_msg("gettimeofday error!", 1));
    }
    arg.start_flag = TRUE;

    while (arg.end_flag < arg.philo_num)
    {
        if (usleep(1000) == -1)
            printf("usleep function is interrupted by a signal\n");
    }

    idx = -1;
    while (++idx < arg.philo_num)
    {
        arg.errno = pthread_mutex_destroy(&arg.fork[idx]);
        if (arg.errno == EBUSY)
        {
            pthread_mutex_unlock(&arg.fork[arg.end_flag]);
            pthread_mutex_destroy(&arg.fork[arg.end_flag]);
        }
        else if (arg.errno == EINVAL)
            printf("The value specified by mutex is invalid.\n");
    }
    arg_free(&arg);
    return (0);
}
