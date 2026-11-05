/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 16:11:00 by ngero             #+#    #+#             */
/*   Updated: 2026/11/05 17:39:20 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"
#include "parser.h"
#include "executor.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

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

static char	*find_binary(char *buf)
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

static void	exec_ast(t_ast *ast)
{
	char	*binary;
	
	binary = find_binary(ast->s_prgrm.argv[0]);
	if (!binary)
	{
		ft_printf("\033[31m%s: Command not found\n\033[0m", ast->s_prgrm.argv[0]);
		exit(1);
	}
	execve(binary, ast->s_prgrm.argv, NULL);
}

int	fork_and_exec(t_ast *ast)
{
	pid_t	f_out;

	f_out = fork();
	if (f_out < 0)
		return (0);
	if (f_out == 0)
		exec_ast(ast);
	else
		waitpid(f_out, NULL, 0);
	return (1);
}

