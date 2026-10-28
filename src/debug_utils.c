/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 18:33:48 by ngero             #+#    #+#             */
/*   Updated: 2026/10/27 18:34:08 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

void	print_buffer(char *buf, size_t buf_size)
{
	size_t	i;

	ft_printf("Buffer(%d): ", buf_size);
	i = 0;
	while (i < buf_size)
	{
		if (ft_isprint(buf[i]))
			ft_putchar(buf[i]);
		else
			ft_printf("0x%x", buf[i]);
		i++;
	}
	ft_putstr("\n");
}
