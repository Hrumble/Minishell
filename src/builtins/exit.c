/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 10:47:10 by ngero             #+#    #+#             */
/*   Updated: 2026/11/21 18:22:03 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

int	builtin_exit(char **av, int ac, char **env)
{
	int	code;

	(void)env;
	if (ac == 2)
		exit(1);
	if (ac > 3)
		return (ft_putstr_fd("exit: too many arguments\n", 2), 1);
	code = ft_atoi(av[0]);
	if (code < 0)
		exit(WEXITSTATUS(156));
	exit(WEXITSTATUS(code));
}
