/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <ngero@student.codam.nl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 08:58:19 by user              #+#    #+#             */
/*   Updated: 2026/11/15 12:52:49 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "ft_printf.h"
#include "lexer.h"
#include "libft.h"
#include "parser.h"
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

int	open_heredoc_file()
{
	int	file;

	file = open(HEREDOC_FILE_PATH, O_RDONLY);
	if (file < 0)
		return (0);
	return (file);
}

int	write_heredoc_file(t_prgrmredir *heredoc_redir)
{
	int	file;

	if (!heredoc_redir || heredoc_redir->type != REDIR_HEREDOC)
		return (0);
	file = open(HEREDOC_FILE_PATH, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (file < 0)
		return (0);
	read_until_delim(file, heredoc_redir->target);
	close(file);
	return (1);
}
