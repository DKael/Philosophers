/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:58:56 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/27 20:13:28 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

inline int	ft_wifexited(int status)
{
	return (((*(int *)&(status)) & 0177) == 0);
}

inline int	ft_wexitstatus(int status)
{
	return (((*(int *)&(status)) >> 8) & 0x000000ff);
}

inline int	ft_wifsignaled(int status)
{
	int	tmp;

	tmp = ((*(int *)&(status)) & 0177);
	return (tmp != 0177 && tmp != 0);
}
