/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <ngero@student.codam.nl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 08:58:19 by user              #+#    #+#             */
/*   Updated: 2026/11/12 10:10:22 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "ft_printf.h"
#include "libft.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <unistd.h>

static int	_is_delim(char *buf, char *delim)
{
	if (!buf)
		return (0);
	return (!ft_strcmp(buf, delim));
}

static int	read_until_delim(int fd, char *delim)
{
	char	*buf;

	buf = readline("> ");
	while (!_is_delim(buf, delim))
	{
		if (buf == 0)
			return (ft_printf(
						"warning: heredoc delimited by EOF, (wanted '%s')\n",
						delim), 1);
		write(fd, buf, ft_strlen(buf));
		write(fd, "\n", 1);
		buf = readline("> ");
	}
	return (1);
}

int	handle_heredocs_redir(char *delim, t_defaultredir *credirs)
{
	int		fd;

	fd = open(HEREDOC_FILE_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (fd < 0)
		return (0);
	restore_default_streams(credirs);
	read_until_delim(fd, delim);
	close(fd);
	save_default_streams(credirs);
	return (pipe_fd_to_file(STDIN_FILENO, HEREDOC_FILE_PATH, O_RDONLY));
}
