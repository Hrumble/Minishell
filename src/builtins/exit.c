/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 10:47:10 by ngero             #+#    #+#             */
/*   Updated: 2026/11/28 18:19:44 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "internal.h"
#include <stdlib.h>

static int	validate_integer(char *buf)
{
	if (*buf == '-' || *buf == '+')
		buf++;
	while (*buf)
		if (!ft_isdigit(*buf++))
			return (0);
	return (1);
}

int	builtin_exit(char **av, int ac, char **env)
{
	int	code;

	(void)env;
	code = 0;
	if (ac == 1)
		exit(get_exit_status());
	if (ac > 2)
		return (ft_putstr_fd("exit: too many arguments\n", 2), 1);
	if (!validate_integer(av[1]))
		return (ft_putstr_fd("exit: numeric argument required\n", 2), 2);
	code = ft_atoi(av[1]);
	if (code < 0)
		exit(255);
	exit(code);
}
