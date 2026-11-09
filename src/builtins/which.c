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
		return (ft_putstr("which: Yeah well specify a binary fuckass.\n"), 1);
	i = 1;
	while (i < ac)
	{
		if (get_builtin(av[i]))
		{
			ft_printf("which: %s is a builtin\n", av[i++]);
			continue ;
		}
		bin = find_binary(av[i]);
		if (!bin)
			return (ft_printf("which: %s not found\n", av[i]), 1);
		ft_printf("%s\n", bin);
		i++;
	}
	return (0);
}
