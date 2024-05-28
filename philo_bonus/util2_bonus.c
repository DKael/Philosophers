/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util2_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/27 16:08:28 by hyungdki          #+#    #+#             */
/*   Updated: 2023/08/27 18:27:39 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int	ft_itoa(int n, char *buf)
{
	int		digit;
	int		temp_n;
	int		digit_save;

	if (n == 0)
	{
		buf[0] = '0';
		buf[1] = '\0';
		return (1);
	}
	temp_n = n;
	digit = 0;
	while (temp_n != 0)
	{
		digit++;
		temp_n /= 10;
	}
	digit_save = digit;
	buf[digit--] = '\0';
	while (n != 0)
	{
		buf[digit--] = (n % 10) + '0';
		n /= 10;
	}
	return (digit_save);
}

void	ft_strcat(char *dest, char *src)
{
	int	dest_index;
	int	src_index;

	dest_index = 0;
	src_index = 0;
	while (dest[dest_index] != '\0')
		dest_index++;
	while (src[src_index] != '\0')
	{
		dest[dest_index] = src[src_index];
		dest_index++;
		src_index++;
	}
	dest[dest_index] = '\0';
}

size_t	ft_strlen(const char *s)
{
	size_t	index;

	index = 0;
	while (s[index] != '\0')
	{
		index++;
	}
	return (index);
}
