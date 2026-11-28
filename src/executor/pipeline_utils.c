/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <ngero@student.codam.nl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/15 12:11:04 by user              #+#    #+#             */
/*   Updated: 2026/11/15 17:16:56 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "parser.h"
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int	count_pipeline_programs(t_ast *pipeline)
{
	if (pipeline->type == AST_BINARYOP)
	{
		return (count_pipeline_programs(pipeline->s_bop.left)
		+ count_pipeline_programs(pipeline->s_bop.right));
	}
	return (1);
}

t_ast	**flatten_pipeline(t_ast *pipeline)
{
	t_ast	**programs;
	t_ast	*current;
	int		i;
	int		size;

	size = count_pipeline_programs(pipeline);
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

int	run_standalone_program(t_ast *prgrm, char **env)
{
	int		io[2];
	pid_t	pid;
	int		(*f)(char **, int, char **);
	
	io[0] = STDIN_FILENO;
	io[1] = STDOUT_FILENO;
	f = NULL;
	if (prgrm->s_prgrm.ac > 0)
		f = get_builtin(prgrm->s_prgrm.argv[0]);
	if (!set_program_heredoc_io(prgrm, io))
		return (0);
	if (f)
		return (run(prgrm, env, io));
	pid = fork_and_exec(prgrm, env, io, STDIN_FILENO);
	await_processes(&pid, 1);
	return (1);
}
