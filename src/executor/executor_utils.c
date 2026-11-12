/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 17:09:06 by ngero             #+#    #+#             */
/*   Updated: 2026/11/12 10:15:50 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "ft_printf.h"
#include "parser.h"
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

void	free_split(char **split)
{
	char	**seeker;

	if (!split)
		return ;
	seeker = split;
	while (*seeker)
		free(*seeker++);
	free(split);
}

int	restore_default_streams(t_defaultredir *credirs)
{
	if (credirs->default_io[0] < 0 || credirs->default_io[1] < 0)
		return (0);
	if (dup2(credirs->default_io[0], 0) < 0)
		return (close(credirs->default_io[0]), 0);
	if (dup2(credirs->default_io[0], 1) < 0)
		return (close(credirs->default_io[1]), 0);
	close(credirs->default_io[0]);
	close(credirs->default_io[1]);
	return (1);
}

int	save_default_streams(t_defaultredir *credirs)
{
	credirs->default_io[0] = dup(STDIN_FILENO);
	if (credirs->default_io[0] < 0)
		return (0);
	credirs->default_io[1] = dup(STDOUT_FILENO);
	if (credirs->default_io[1] < 0)
	{
		dup2(credirs->default_io[0], STDIN_FILENO);
		return (close(credirs->default_io[0]), 0);
	}
	return (1);
}

void	await_processes(pid_t *processes, int size)
{
	int	i;
	
	if (size >= MAX_PROCESSES)
		return ;
	i = 0;
	while (i < size)
	{
		if (processes[i] != 1)
			waitpid(processes[i], NULL, 0);
		i++;
	}
}

int	run_builtin(int (*builtin)(char **, int, char **), t_ast *prgrm,
		char **env, int fd[2])
{
	t_defaultredir	credirs;

	if (!setup_pipeline_redirections(fd)
		|| !setup_prgrm_redirections(prgrm, &credirs))
	{
		ft_putstr("\033[31mRedirection failed\033[0m\n");
		return (1);
	}
	builtin(prgrm->s_prgrm.argv, prgrm->s_prgrm.ac, env);
	if (!restore_default_streams(&credirs))
		ft_putstr("\033[31mFailed to restore standard streams!\033[0m\n");
	return (1);
}
