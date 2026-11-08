/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 16:11:00 by ngero             #+#    #+#             */
/*   Updated: 2026/11/08 19:03:06 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "builtins.h"
#include "libft.h"
#include "parser.h"
#include "executor.h"
#include <stddef.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>


static int	_exec_ast(t_ast *ast, char **env)
{
	char	*binary;
	
	if (!ast || ast->type != AST_PRGRM)
		return (0);
	binary = find_binary(ast->s_prgrm.argv[0]);
	if (!binary)
	{
		ft_printf("\033[31m%s: Command not found\n\033[0m", ast->s_prgrm.argv[0]);
		free_ast(&ast);
		exit(1);
	}
	ast->s_prgrm.argv[ast->s_prgrm.ac] = 0;
	return (execve(binary, ast->s_prgrm.argv, env));
}

static void	_child_process(t_ast *ast, char **env, int fd[2])
{
	t_currentredirs	credirs;

	if (!setup_pipeline_redirections(fd)
		|| !setup_prgrm_redirections(ast, &credirs, 0))
	{
		ft_putstr("\033[31mRedirection failed\033[0m\n");
		free_ast(&ast);
		exit(1);
	}
	_exec_ast(ast, env);
	exit(0);
}

int	_exec_builtin(t_ast *prgrm, char **env, int fd[2])
{
	if (!ft_strcmp(prgrm->s_prgrm.argv[0], "echo"))
		return (run_builtin(builtin_echo, prgrm, env, fd));
	if (!ft_strcmp(prgrm->s_prgrm.argv[0], "cd"))
		return (run_builtin(builtin_cd, prgrm, env, fd));
	if (!ft_strcmp(prgrm->s_prgrm.argv[0], "exit"))
		return (run_builtin(builtin_exit, prgrm, env, fd));
	if (!ft_strcmp(prgrm->s_prgrm.argv[0], "which"))
		return (run_builtin(builtin_which, prgrm, env, fd));
	return (0);
}

int	fork_and_exec(t_ast *ast, char **env, int fd[2])
{
	pid_t	f_out;

	if (ast->type == AST_BINARYOP)
		return (execute_pipeline(ast, env));
	f_out = fork();
	if (f_out < 0)
		return (0);
	if (f_out == 0)
		_child_process(ast, env, fd);
	return (f_out);
}
