/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 17:09:06 by ngero             #+#    #+#             */
/*   Updated: 2026/11/05 17:16:04 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "parser.h"
#include <stdlib.h>

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

void	handle_redir(t_ast *prgrm)
{
	if (!prgrm || prgrm->type != AST_PRGRM)
		return ;
}

int	run_builtin(int (*builtin)(char **, int), char **av, int ac)
{
	int	out;

	out = builtin(av, ac);
	return (1);
}

