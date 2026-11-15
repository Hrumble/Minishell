/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <ngero@student.codam.nl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/08 15:27:45 by user              #+#    #+#             */
/*   Updated: 2026/11/15 12:52:01 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "parser.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static void	_close_io(int io[2])
{
	if (io[0] != STDIN_FILENO)
		close(io[0]);
	if (io[1] != STDOUT_FILENO)
		close(io[1]);
}

static int	_set_program_heredoc_io(t_ast *program, int io[2])
{
	int	file;
	int	i;

	i = 0;
	while (i < program->s_prgrm.redir_size)
	{
		if (!write_heredoc_file(program->s_prgrm.redir[i]))
			return (0);
		if (io[0] != STDIN_FILENO)
			close(io[0]);
		file = open_heredoc_file();
		if (!file)
			return (0);
		io[0] = file;
		i++;
	}
	return (1);
}

static int	_set_program_io(t_ast **programs, int io[2], int *prev)
{
	int	pipe_io[2];

	io[0] = *prev;
	io[1] = STDOUT_FILENO;
	if ((*programs)->s_prgrm.redir_size > 0)
		if (!_set_program_heredoc_io(*programs, io))
			return (0);
	if (programs[1])
	{
		if (pipe(pipe_io) < 0)
			return (0);
		io[1] = pipe_io[1];
	}
	*prev = pipe_io[0];
	return (1);
}

static int	_pipe_and_execute(t_ast **programs, pid_t *processes, char **env)
{
	int	io[2];
	int	i;
	int	prev;

	prev = STDIN_FILENO;
	i = 0;
	while (programs[i])
	{
		if (!_set_program_io(programs + i, io, &prev))
			return (_close_io(io), 0);
		processes[i] = fork_and_exec(programs[i], env, io);
		_close_io(io);
		i++;
	}
	return (i);
}

int	execute_pipeline(t_ast	*pipeline, char **env)
{
	t_ast	**programs;
	int		n_proc;
	pid_t	processes[MAX_PROCESSES];

	if (pipeline->type == AST_PRGRM)
		if (exec_builtin(pipeline, env,
					(int[2]){STDIN_FILENO, STDOUT_FILENO}))
			return (1);
	programs = flatten_pipeline(pipeline);
	if (!programs)
		return (0);
	n_proc = _pipe_and_execute(programs, processes, env);
	if (!n_proc)
		return (free(programs), 0);
	await_processes(processes, n_proc);
	return (free(programs), 1);
}
