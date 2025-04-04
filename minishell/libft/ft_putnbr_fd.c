/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 20:44:36 by samatsum          #+#    #+#             */
/*   Updated: 2025/03/25 22:17:08 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	putabsnbr_fd(unsigned int n, int fd)
{
	char	c;

	if (n >= 10)
		putabsnbr_fd(n / 10, fd);
	c = n % 10 + '0';
	ft_putchar_fd((char)c, fd);
}

// why this is ok when n is INT_MIN?
// putabsnbr_fd(-n, fd);
// Don't fully understand type conversion behaviours...
void	ft_putnbr_fd(int n, int fd)
{
	int	tmp;

	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		if (n <= -10)
		{
			tmp = -(n / 10);
			putabsnbr_fd((unsigned int)tmp, fd);
		}
		tmp = -(n % 10) + '0';
		ft_putchar_fd((char)tmp, fd);
	}
	else
		putabsnbr_fd((unsigned int)n, fd);
}
