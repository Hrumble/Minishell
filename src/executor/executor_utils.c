/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 17:09:06 by ngero             #+#    #+#             */
/*   Updated: 2026/11/08 18:13:23 by user             ###   ########.fr       */
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

int	restore_inout(t_currentredirs *credirs)
{
	if (credirs->kbfd < 0 || credirs->termfd < 0)
		return (0);
	if (dup2(credirs->kbfd, 0) < 0)
		return (close(credirs->kbfd), 0);
	if (dup2(credirs->termfd, 1) < 0)
		return (close(credirs->termfd), 0);
	close(credirs->kbfd);
	close(credirs->termfd);
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
	t_currentredirs	credirs;

	if (!setup_pipeline_redirections(fd)
		|| !setup_prgrm_redirections(prgrm, &credirs, 1))
	{
		ft_putstr("\033[31mRedirection failed\033[0m\n");
		return (1);
	}
	builtin(prgrm->s_prgrm.argv, prgrm->s_prgrm.ac, env);
	if (!restore_inout(&credirs))
		ft_putstr("\033[31mFailed to restore standard streams!\033[0m\n");
	return (1);
}
