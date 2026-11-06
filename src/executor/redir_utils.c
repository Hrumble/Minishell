/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 14:54:13 by ngero             #+#    #+#             */
/*   Updated: 2026/11/06 16:59:54 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "lexer.h"
#include "parser.h"
#include <fcntl.h>
#include <unistd.h>

int	pipe_redirection(t_prgrmredir *redir)
{
	if (redir->type == REDIR_IN)
		return (pipe_fd_to_file(0, redir->target, O_RDONLY)); 
	if (redir->type == REDIR_OUT)
		return (pipe_fd_to_file(1, redir->target, O_WRONLY | O_CREAT)); 
	if (redir->type == REDIR_APPENDOUT)
		return (pipe_fd_to_file(1, redir->target, O_WRONLY | O_APPEND | O_CREAT)); 
	return (0);
}

int	setup_prgrm_redirections(t_ast *ast, t_currentredirs *credirs, int builtin)
{
	int	rsize;

	if (!ast || ast->type != AST_PRGRM)
		return (0);
	if (builtin)
	{
		credirs->kbfd = dup(0);
		if (credirs->kbfd < 0)
			return (0);
		credirs->termfd = dup(1);
		if (credirs->termfd < 0)
		{
			dup2(credirs->kbfd, 0);
			return (close(credirs->kbfd), 0);
		}
	}
	rsize = 0;
	while (rsize < ast->s_prgrm.redir_size)
		if (!pipe_redirection(ast->s_prgrm.redir[rsize++]))
		{
			restore_inout(credirs);
			return (0);
		}
	return (1);
}

int	pipe_fd_to_file(int fd, char *path, int mode)
{
	int	ffd;

	ffd = open(path, mode, 0644);
	if (ffd < 0)
		return (0);
	if (dup2(ffd, fd) < 0)
		return (0);
	close(ffd);
	return (1);
}
