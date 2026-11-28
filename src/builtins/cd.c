/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 17:53:17 by ngero             #+#    #+#             */
/*   Updated: 2026/11/21 18:17:57 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "internal.h"
#include "libft.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int	_set_pwd()
{
	char	*old_dir;
	char	*old_key;
	char	*new_dir;
	char	*new_key;

	old_dir = vtable_get("PWD");
	old_key = "OLDPWD";
	new_key = "PWD";
	new_dir = getcwd(NULL, 0);
	if (!new_dir)
		return (free(old_dir), 0);
	if (!seek_variable("OLDPWD"))
		old_key = ft_strdup("OLDPWD");
	if (!seek_variable("PWD"))
		new_key = ft_strdup("PWD");
	if (!declare_variable(old_key, old_dir, 1))
		return (0);
	return (declare_variable(new_key, new_dir, 1));
}

static int	_set_dir(char *dir)
{
	if (*dir == '-')
	{
		dir = vtable_get("OLDPWD");
		if (!dir)
			return (0);
	}
	if (chdir(dir) < 0)
	{
		perror("cd");
		return (0);
	}
	if (!_set_pwd())
		return (0);
	return (1);
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
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (1);
	}
	return (!_set_dir(av[1]));
}

