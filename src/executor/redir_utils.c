/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 14:54:13 by ngero             #+#    #+#             */
/*   Updated: 2026/11/12 10:35:54 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "lexer.h"
#include "parser.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int	set_redirection_stream(t_prgrmredir *redir, t_defaultredir *credirs)
{
	if (redir->type == REDIR_IN)
		return (pipe_fd_to_file(STDIN_FILENO,
					redir->target, O_RDONLY)); 
	if (redir->type == REDIR_OUT)
		return (pipe_fd_to_file(STDOUT_FILENO,
					redir->target, O_WRONLY | O_CREAT)); 
	if (redir->type == REDIR_APPENDOUT)
		return (pipe_fd_to_file(STDOUT_FILENO,
					redir->target, O_WRONLY | O_APPEND | O_CREAT)); 
	if (redir->type == REDIR_HEREDOC)
		return (handle_heredocs_redir(redir->target, credirs));
	return (0);
}

int	setup_prgrm_redirections(t_ast *ast, t_defaultredir *credirs)
{
	int	rsize;

	if (!ast || ast->type != AST_PRGRM)
		return (0);
	rsize = 0;
	while (rsize < ast->s_prgrm.redir_size)
		if (!set_redirection_stream(ast->s_prgrm.redir[rsize++], credirs))
			return (restore_default_streams(credirs), 0);
	return (1);
}

int	pipe_fd_to_file(int fd, char *path, int mode)
{
	int	ffd;

	ffd = open(path, mode, 0644);
	if (ffd < 0)
		return (perror("failed to open file: "), 0);
	if (dup2(ffd, fd) < 0)
		return (0);
	close(ffd);
	return (1);
}
