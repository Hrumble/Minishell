/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 14:41:45 by ngero             #+#    #+#             */
/*   Updated: 2026/11/21 15:56:35 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "ft_printf.h"

static int	_is_n_flag(char *buf)
{
	if (*buf++ != '-')
		return (0);
	if (*buf != 'n')
		return (0);
	while (*buf == 'n')
		buf++;
	return (!*buf);
}

int	builtin_echo(char **av, int ac, char **env)
{
	int	n_flag;
	int	i;

	(void)env;
	n_flag = 0;
	i = 1;
	while (ac > i && _is_n_flag(av[i]))
	{
		if (_is_n_flag(av[i]))
			n_flag = 1;
		else
			break;
		i++;
	}
	while (i < ac)
	{
		if (i != ac - 1)
			ft_printf("%s ", av[i++]);
		else
			ft_printf("%s", av[i++]);
	}
	if (!n_flag)
		ft_putchar('\n');
	return (0);
}
