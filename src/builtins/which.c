/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   which.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 17:59:46 by ngero             #+#    #+#             */
/*   Updated: 2026/11/06 17:19:36 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "ft_printf.h"

int	builtin_which(char **av, int ac, char **env)
{
	int		i;
	char	*bin;

	(void)env;
	if (ac < 2)
		return (ft_putstr("Yeah well specify a binary fuckass.\n"), 0);
	i = 1;
	while (i < ac)
	{
		bin = find_binary(av[i]);
		if (!bin)
			return (ft_printf("%s not found\n", av[i]));
		ft_printf("%s\n", bin);
		i++;
	}
	return (1);
}
