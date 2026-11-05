/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 16:11:00 by ngero             #+#    #+#             */
/*   Updated: 2026/11/05 17:52:48 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "builtins.h"
#include "libft.h"
#include "parser.h"
#include "executor.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

static int	_run_builtin(int (*builtin)(char **, int), char **av, int ac)
{
	int	out;

	out = builtin(av, ac);
	return (1);
}

static int	_exec_builtin(char **av, int ac)
{
	if (!ft_strcmp(av[0], "echo"))
		return (_run_builtin(builtin_echo, av, ac));
	if (!ft_strcmp(av[0], "cd"))
		return (_run_builtin(builtin_cd, av, ac));
	if (!ft_strcmp(av[0], "exit"))
		return (_run_builtin(builtin_exit, av, ac));
	return (0);
}

static void	_exec_ast(t_ast *ast)
{
	char	*binary;
	
	if (_exec_builtin(ast->s_prgrm.argv, ast->s_prgrm.ac))
		return ;
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
		_exec_ast(ast);
	else
		waitpid(f_out, NULL, 0);
	return (1);
}

