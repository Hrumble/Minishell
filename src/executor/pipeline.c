/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <ngero@student.codam.nl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/08 15:27:45 by user              #+#    #+#             */
/*   Updated: 2026/11/08 18:04:03 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "parser.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int	_count_pipeline_programs(t_ast *pipeline)
{
	if (pipeline->type == AST_BINARYOP)
	{
		return (_count_pipeline_programs(pipeline->s_bop.left)
		+ _count_pipeline_programs(pipeline->s_bop.right));
	}
	return (1);
}

// Flattens the pipeline, returns a null terminated array of each
// program to be ran in order.
// When freeing, do not free the ast nodes, they are references from the actual
// AST
static t_ast	**_flatten_pipeline(t_ast *pipeline)
{
	t_ast	**programs;
	t_ast	*current;
	int		i;
	int		size;

	size = _count_pipeline_programs(pipeline);
	programs = malloc((size + 1) * sizeof(t_ast *));
	if (!programs)
		return (0);
	programs[size] = 0;
	i = size - 1;
	current = pipeline;
	while (current->type == AST_BINARYOP)
	{
		programs[i--] = current->s_bop.right;
		current = current->s_bop.left;
	}
	programs[i] = current;
	return (programs);
}

int	setup_pipeline_redirections(int fd[2])
{
	if (fd[0] != STDIN_FILENO)
	{
		if (dup2(fd[0], 0) < 0)
			return (close(fd[0]), close(fd[1]), 0);
		close(fd[0]);
	}
	if (fd[1] != STDOUT_FILENO)
	{
		if (dup2(fd[1], 1) < 0)
			return (close(fd[1]), 0);
		close(fd[1]);
	}
	return (1);
}

// returns the number of processes
static int	pipe_and_execute(t_ast **programs, pid_t *processes, char **env)
{
	int	io[2];
	int	fd[2];
	int	i;
	int	prev;

	prev = STDIN_FILENO;
	i = 0;
	while (programs[i])
	{
		io[0] = prev;
		io[1] = STDOUT_FILENO;
		if (programs[i + 1])
		{
			if (pipe(fd) < 0)
				return (0);
			io[1] = fd[1];
		}
		processes[i] = fork_and_exec(programs[i], env, io);
		if (io[0] != STDIN_FILENO)
			close(io[0]);
		if (io[1] != STDOUT_FILENO)
			close(io[1]);
		prev = fd[0];
		i++;
	}
	return (i);
}

// fd is 5 instead of 2, but hear me out
// say fd is [a, b, c, d, e]
// a: fd[0]
// b: fd[1]
// c: io[0]
// d: io[1]
// e: prev_in
int	execute_pipeline(t_ast	*pipeline, char **env)
{
	t_ast	**programs;
	int		n_proc;
	pid_t	processes[MAX_PROCESSES];

	if (pipeline->type != AST_BINARYOP)
		if (_exec_builtin(pipeline, env,
					(int[2]){STDIN_FILENO, STDOUT_FILENO}))
			return (1);
	programs = _flatten_pipeline(pipeline);
	if (!programs)
		return (0);
	n_proc = pipe_and_execute(programs, processes, env);
	if (!n_proc)
		return (free(programs), 0);
	await_processes(processes, n_proc);
	return (free(programs), 1);
}

