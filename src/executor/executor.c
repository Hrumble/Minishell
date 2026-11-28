/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 16:11:00 by ngero             #+#    #+#             */
/*   Updated: 2026/11/28 18:22:19 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"
#include "libft.h"
#include "parser.h"
#include "executor.h"
#include <stddef.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>


// calls execve with the argv and argc of the provided ast program
static int	_exec_ast(t_ast *ast, char **env)
{
	int		res;
	char	*binary;
	
	if (ast->s_prgrm.ac == 0)
		return (0);
	res = find_binary(ast->s_prgrm.argv[0], &binary);
	if (res != 1)
	{
		ft_putstr_fd(ast->s_prgrm.argv[0], 2);
		if (res == ACC_NOEXIST)
			return (ft_putstr_fd(": No such file or directory\n", 2), 127);
		if (res == ACC_NOPERM)
			return (ft_putstr_fd(": Permission denied\n", 2), 126);
		if (res == ACC_NOTFILE)
			return (ft_putstr_fd(": Is a directory\n", 2), 126);
		return (ft_putstr_fd(": command not found\n", 2), 127);
	}
	ast->s_prgrm.argv[ast->s_prgrm.ac] = 0;
	return (execve(binary, ast->s_prgrm.argv, env));
}

// Executes the builtin function specified by argv[0] of `prgrm`
// returns 0 if it does not manage to find a builtin, 1 otherwise
static int	_exec_builtin(t_ast *prgrm, char **env, int fd[2])
{
	t_defaultredir	credirs;
	int	(*f)(char **, int, char **);

	f = NULL;
	save_default_streams(&credirs);
	if (!set_io_streams(fd)
		|| !setup_prgrm_redirections(prgrm))
	{
		restore_default_streams(&credirs);
		set_exit_status(1);
		return (1);
	}
	if (prgrm->s_prgrm.ac > 0)
	{
		f = get_builtin(prgrm->s_prgrm.argv[0]);
		set_exit_status(f(prgrm->s_prgrm.argv, prgrm->s_prgrm.ac, env));
	}
	if (!restore_default_streams(&credirs))
		ft_putstr_fd("\033[31mFailed to restore standard streams!\033[0m\n", 2);
	return (0);
}

int	run(t_ast *ast, char **env, int io[2])
{
	int	(*f)(char **, int, char **);

	f = NULL;
	if (ast->s_prgrm.ac > 0)
		f = get_builtin(ast->s_prgrm.argv[0]);
	if (!f)
	{
		if (!set_io_streams(io)
			|| !setup_prgrm_redirections(ast))
			return (1);
		return (_exec_ast(ast, env));
	}
	return (_exec_builtin(ast, env, io));
}

int	fork_and_exec(t_ast *ast, char **env, int fd[2], int prev)
{
	int		out;
	pid_t	f_out;

	f_out = fork();
	if (f_out < 0)
		return (0);
	if (f_out == 0)
	{
		if (prev != STDIN_FILENO)
			close(prev);
		out = run(ast, env, fd);
		free_ast(&ast);
		free_split(env);
		free_vtable();
		exit(out);
	}
	return (f_out);
}
