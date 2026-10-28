/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 18:33:05 by ngero             #+#    #+#             */
/*   Updated: 2026/10/28 17:46:07 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"
#include "internal.h"
#include <stdlib.h>

static t_cmdline	*new_cmdline(char **av, int ac)
{
	t_cmdline	*cmdline;

	cmdline = malloc(sizeof(t_cmdline));
	if (!cmdline)
		return (0);
	cmdline->av = av;
	cmdline->ac = ac;
	return (cmdline);
}

void		free_cmdline(t_cmdline *cmdline)
{
	if (!cmdline)
		return ;
	free_split(cmdline->av);
	free(cmdline);
}

t_cmdline	*parse_command_line(const char *c)
{
	int		ac;
	char	**av;

	if (!c)
		return (0);
	av = split_cmdline(c);
	ac = count_split((const void **)av);
	if (!av)
		return (0);
	return (new_cmdline(av, ac));
}

int	interpret_command_line(t_cmdline *cmdline)
{
	if (!ft_strcmp(*cmdline->av, "echo"))
		return (builtin_echo(cmdline->av, cmdline->ac));
	if (!ft_strcmp(*cmdline->av, "exit"))
		return (builtin_exit(cmdline->av, cmdline->ac));
	return (0);
}

