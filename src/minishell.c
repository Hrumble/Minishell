/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 14:55:42 by ngero             #+#    #+#             */
/*   Updated: 2026/10/24 02:17:23 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "terminal.h"
#include "ft_printf.h"
#include "libft.h"
#include <readline/readline.h>
#include <stddef.h>
#include <unistd.h>

static void	write_buffer_to_screen(char *buf, size_t size)
{
	terminal_send_command("[2K\r");
	ft_putstr("sex shell v1.0> ");
	write(1, buf, size);
}

static int	get_user_input(char *buf)
{
	int	buf_size;
	int	n_read;

	ft_putstr("sex shell v1.0> ");
	n_read = read(STDIN_FILENO, buf, 10);
	buf_size = n_read;
	while (n_read > 0 && *buf != '\n')
	{
		write_buffer_to_screen(buf, buf_size);
		if (*buf != '\033')
		{
			buf += n_read;
			buf_size += n_read;
		}
		n_read = read(STDIN_FILENO, buf, 10);
	}
	buf[1] = 0;
	if (n_read < 0)
		return (0);
	return (1);
}

void	minishell()
{
	char	buf[1024];

	terminal_set_icanon(0);
	while (get_user_input(buf))
		ft_putstr_fd(buf, 1);
}
