/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_sem_func_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/27 15:55:58 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/27 16:18:30 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	sem_wait_nointr(sem_t *sem)
{
	int	result;

	result = sem_wait(sem);
	while (result == -1)
		result = sem_wait(sem);
}

void	ft_sem_destroy(t_csem *csem)
{
	sem_close(csem->sem);
	sem_unlink(csem->name);
}

sem_t	*ft_sem_open(const char *name, mode_t mode, unsigned int value)
{
	sem_t	*temp;

	temp = sem_open(name, O_EXCL | O_CREAT, mode, value);
	if (temp == SEM_FAILED)
	{
		sem_unlink(name);
		temp = sem_open(name, O_EXCL | O_CREAT, mode, value);
	}
	return (temp);
}

int	make_multiple_sem(t_csem *lst, const char *name, int *cnt, int num)
{
	char	*temp;

	*cnt = -1;
	while (++(*cnt) < num)
	{
		temp = ft_itoa((*cnt));
		if (temp == T_NULL)
			return (1);
		lst[(*cnt)].name = ft_strjoin(name, temp);
		free(temp);
		if (lst[(*cnt)].name == T_NULL)
			return (1);
		lst[(*cnt)].sem = ft_sem_open(lst[(*cnt)].name, 0644, 1);
		if (lst[(*cnt)].sem == SEM_FAILED)
		{
			free(lst[(*cnt)].name);
			return (1);
		}	
	}
	return (0);
}
