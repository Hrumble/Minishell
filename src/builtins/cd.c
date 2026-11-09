/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 17:53:17 by ngero             #+#    #+#             */
/*   Updated: 2026/11/08 19:12:17 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "ft_printf.h"
#include <stdlib.h>
#include <unistd.h>

static int	_set_dir(char *dir)
{
	if (chdir(dir) < 0)
	{
		ft_printf("cd: where are you even trying to go man\n");
		return (1);
	}
	return (0);
}

int	builtin_cd(char **av, int ac, char **env)
{
	char	*home;

	(void)env;
	(void)av;
	if (ac < 2)
	{
		home = getenv("HOME");
		if (!home)
			return (1);
		return (_set_dir(home));
	}
	if (ac > 2)
	{
		ft_printf("cd: wayyyy too many arguments\n");
		return (1);
	}
	_set_dir(av[1]);
	return (0);
}

