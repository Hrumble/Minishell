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

int	unset_var(char *buf)
{
	t_variable	*var;

	var = remove_variable(buf);
	free_variable(var);
	return (1);
}

int	builtin_unset(char **av, int ac, char **env)
{
	int	i;

	(void)env;
	i = 0;
	while (i < ac)
		unset_var(av[i++]);
	return (0);
}
