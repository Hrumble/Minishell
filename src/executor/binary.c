/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 17:47:50 by ngero             #+#    #+#             */
/*   Updated: 2026/11/05 17:48:10 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "executor.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

static char	*find_binary_in_path(char *buf)
{
	char	concat[PATH_SIZE];
	char	**c_path;
	char	**path;
	int		cpy_out;

	path = ft_split(getenv("PATH"), ':');
	if (!path)
		return (0);
	c_path = path;
	while (*c_path)
	{
		cpy_out = ft_strlcpy(concat, *c_path, PATH_SIZE); 
		if (cpy_out > PATH_SIZE - 1)
			return (free_split(path), (void *)0);
		concat[cpy_out] = '/';
		concat[cpy_out + 1] = '\0';
		if (ft_strlcat(concat, buf, PATH_SIZE) > PATH_SIZE)
			return (free_split(path), (void *)0);
		if (!access(concat, X_OK | R_OK))
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
	return (find_binary_in_path(buf));
}
