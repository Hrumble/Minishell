/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <ngero@student.codam.nl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 08:58:19 by user              #+#    #+#             */
/*   Updated: 2026/11/15 17:46:19 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "ft_printf.h"
#include "internal.h"
#include "lexer.h"
#include "libft.h"
#include "parser.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

static int	_is_delim(char *buf, char *delim)
{
	if (!buf)
		return (0);
	return (!ft_strcmp(buf, delim));
}

// Expands the delimiter and returns 1 if the delimiter was in quotes
// 0 otherwise
static int	_expand_delimiter(char **delim)
{
	char	*expanded;
	size_t	len;

	len = ft_strlen(*delim);
	expanded = expand_buf(*delim, 0);
	if (!expanded)
		return (-1);
	*delim = expanded;
	return (ft_strlen(expanded) == len);
}

// Reads a line, returns the expanded buffer, or NULL on EOF
static char	*_read_and_expand(char *delim, int should_expand)
{
	char	*expanded;
	char	*buf;

	buf = readline("> ");
	if (!buf)
	{
		ft_printf("warning: heredoc delimited by EOF, (wanted '%s')\n",
			delim);
		return (NULL);
	}
	if (_is_delim(buf, delim))
		return (free(buf), NULL);
	if (should_expand)
	{
		expanded = expand_buf(buf, 1);
		free(buf);
		return (expanded);
	}
	return (buf);
}

static int	_read_until_delim(int fd, char *delim)
{
	char	*buf;
	int		should_expand;

	should_expand = _expand_delimiter(&delim);
	if (should_expand < 0)
		return (0);
	buf = _read_and_expand(delim, should_expand);
	while (buf)
	{
		write(fd, buf, ft_strlen(buf));
		write(fd, "\n", 1);
		free(buf);
		buf = _read_and_expand(delim, should_expand);
	}
	free(delim);
	return (1);
}

int	write_heredoc_file(t_prgrmredir *heredoc_redir)
{
	int	pipe_io[2];

	if (!heredoc_redir || heredoc_redir->type != REDIR_HEREDOC)
		return (-1);
	if (pipe(pipe_io) < 0)
		return (-1);
	_read_until_delim(pipe_io[1], heredoc_redir->target);
	close(pipe_io[1]);
	return (pipe_io[0]);
}
