/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <ngero@student.codam.nl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/21 18:10:56 by user              #+#    #+#             */
/*   Updated: 2026/11/21 18:15:23 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"

static void	_unset_var(char *buf)
{
	remove_variable(buf);
}

int	builtin_unset(char **av, int ac, char **env)
{
	int	i;

	(void)env;
	i = 1;
	while (i < ac)
		_unset_var(av[i++]);
	return (0);
}
