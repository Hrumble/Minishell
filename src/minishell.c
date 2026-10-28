/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 17:58:51 by ngero             #+#    #+#             */
/*   Updated: 2026/10/27 20:59:46 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_printf.h"
#include "terminal.h"
#include <stddef.h>
#include <unistd.h>


int	minishell()
{
	char		buf[1024];
	t_cmdline	*cmdline;

	terminal_set_icanon(0);
	while (1)
	{
		get_user_input("metasex> ", buf);
		cmdline = parse_command_line(buf);
		if (!cmdline)
			ft_printf("\033[31mUnknown error occured\033[0m\n");
		else
			if (!interpret_command_line(cmdline))
				ft_printf("\033[31mUnknown command: %s\033[0m\n", buf);
		free_cmdline(cmdline);
	}
	return (1);
}
