/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/24 00:50:09 by ngero             #+#    #+#             */
/*   Updated: 2026/10/24 01:49:47 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "termios.h"
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include "stdarg.h"

int	terminal_set_icanon(int icanon)
{
	struct termios	attr;

	tcgetattr(STDIN_FILENO, &attr);
	if (icanon)
	{
		attr.c_lflag |= (ICANON | ECHO);
	}
	else
		attr.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(0, TCSANOW, &attr);
	return (1);
}

void	terminal_send_command(const char *command)
{
	if (!command)
		return ;
	write(1, "\033[", 1);
	write(1, command, ft_strlen(command));
}

void	terminal_send_command_count(const char *command, int count)
{
	if (!command)
		return ;
	write(1, "\033[", 2);
	ft_putnbr_fd(count, 1);
	write(1, command, ft_strlen(command));
}
