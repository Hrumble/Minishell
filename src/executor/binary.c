/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 17:47:50 by ngero             #+#    #+#             */
/*   Updated: 2026/11/21 18:14:50 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "executor.h"
#include "internal.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

static int	_build_binary_path(char *cmd, char *path, char *bin)
{
	int	cpy_out;

	cpy_out = ft_strlcpy(bin, path, PATH_SIZE); 
	if (cpy_out > PATH_SIZE - 1)
		return (0);
	bin[cpy_out] = '/';
	bin[cpy_out + 1] = '\0';
	if (ft_strlcat(bin, cmd, PATH_SIZE) > PATH_SIZE)
		return (0);
	return (1);
}

static char	*_find_binary_in_path(char *buf)
{
	char	concat[PATH_SIZE];
	char	**c_path;
	char	*envpath;
	char	**path;

	envpath = vtable_get("PATH");
	if (!envpath)
		return (0);
	path = ft_split(envpath, ':');
	if (!path)
		return (free(envpath), NULL);
	free(envpath);
	c_path = path;
	while (*c_path)
	{
		if (!_build_binary_path(buf, *c_path, concat))
			return (free_split(path), NULL);
		if (file_access(concat, X_OK | R_OK))
			return (free_split(path), ft_strdup(concat));
		c_path++;
	}
	return (free_split(path), (void *)0);
}

int (*get_builtin(char *buf))(char **, int, char **)
{
	if (!ft_strcmp(buf, "echo"))
		return (builtin_echo);
	if (!ft_strcmp(buf, "cd"))
		return (builtin_cd);
	if (!ft_strcmp(buf, "exit"))
		return (builtin_exit);
	if (!ft_strcmp(buf, "which"))
		return (builtin_which);
	if (!ft_strcmp(buf, "env"))
		return (builtin_env);
	if (!ft_strcmp(buf, "export"))
		return (builtin_export);
	if (!ft_strcmp(buf, "unset"))
		return (builtin_unset);
	return (0);
}

char	*find_binary(char *buf)
{
	if (*buf == '.' || *buf == '/')
	{
		if (access(buf, X_OK | R_OK))
			return (0);
		else
			return (buf);
	}
	return (_find_binary_in_path(buf));
}
