/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 16:11:00 by ngero             #+#    #+#             */
/*   Updated: 2026/11/21 21:36:50 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
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
	char		*binary;
	
	binary = find_binary(ast->s_prgrm.argv[0]);
	if (!binary)
	{
		ft_putstr_fd(ast->s_prgrm.argv[0], 2);
		ft_putstr_fd(": program not found\n", 2);
		return (0);
	}
	ast->s_prgrm.argv[ast->s_prgrm.ac] = 0;
	return (execve(binary, ast->s_prgrm.argv, env));
}

// The code that gets executed by a forked child
static int	_child_process(t_ast *ast, char **env, int io[2])
{
	if (exec_builtin(ast, env, io))
	{
		free_ast(&ast);
		free_split(env);
		free_vtable();
		exit(0);
	}
	if (!set_io_streams(io)
		|| !setup_prgrm_redirections(ast))
		return (0);
	return (_exec_ast(ast, env));
}

// Executes the builtin function specified by argv[0] of `prgrm`
// returns 0 if it does not manage to find a builtin, 1 otherwise
int	exec_builtin(t_ast *prgrm, char **env, int fd[2])
{
	int	(*f)(char **, int, char **);

	f = get_builtin(prgrm->s_prgrm.argv[0]);
	if (!f)
		return (0);
	run_builtin(f, prgrm, env, fd);
	return (1);
}

int	fork_and_exec(t_ast *ast, char **env, int fd[2])
{
	pid_t	f_out;

	if (ast->type != AST_PRGRM)
		return (0);
	f_out = fork();
	if (f_out < 0)
		return (0);
	if (f_out == 0)
	{
		_child_process(ast, env, fd);
		free_ast(&ast);
		free_split(env);
		free_vtable();
		exit(127);
	}
	return (f_out);
}
