/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 17:09:06 by ngero             #+#    #+#             */
/*   Updated: 2026/11/21 21:37:04 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "ft_printf.h"
#include "libft.h"
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
	if (dup2(credirs->default_io[1], 1) < 0)
		return (close(credirs->default_io[1]), 0);
	close(credirs->default_io[0]);
	credirs->default_io[0] = -1;
	close(credirs->default_io[1]);
	credirs->default_io[1] = -1;
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
	int			i;
	extern int	g_exit_status;
	
	if (size >= MAX_PROCESSES)
		return ;
	i = 0;
	while (i < size)
	{
		if (processes[i] != 1)
		{
			waitpid(processes[i], &g_exit_status, 0);
			if (WIFEXITED(g_exit_status))
				g_exit_status = WEXITSTATUS(g_exit_status);
			else if (WIFSIGNALED(g_exit_status))
				g_exit_status = 128 + WTERMSIG(g_exit_status);
		}
		i++;
	}
}
