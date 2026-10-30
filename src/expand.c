/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 16:17:38 by ngero             #+#    #+#             */
/*   Updated: 2026/10/30 18:43:56 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "internal.h"
#include "parser.h"
#include <stdlib.h>

static char	*expand_env(char *buf)
{
	char	*end_ptr;
	char	path_var[20];
	char	*var;

	if (!buf || *buf != '$')
		return (0);
	end_ptr = buf + 1;
	while (ft_iswrd(*end_ptr) && *end_ptr != '$')
		path_var[end_ptr - buf - 1] = *end_ptr;
	path_var[end_ptr - buf - 1] = 0;
	var = getenv(path_var);
	if (!var)
		return (ft_strdup(""));
	return (var);
}

static char	*expand_arg(t_ast *prgrm, char *buf)
{
	int	is_in_squotes;

	is_in_squotes = 0;
	if (*buf == '\'' || *buf == '"')
	{
		if (!find_matching_char(buf))
			return (0);
		if (*buf == '\'')
			is_in_squotes = 1;
		buf++;
	}
	while (*buf)
	{
		if (*buf == '$' && !is_in_squotes)
			prgrm->s_astprgrm.argv[prgrm->s_astprgrm.ac++] = expand_env(buf);
	}
	return (0);
}

int	expand_argv(t_ast	**prgrm)
{
	int		ac;
	char	**av;
	int		i;
	t_ast	*n_prgrm;

	if (!prgrm)
		return (0);
	n_prgrm = new_astprgrm();
	if (!n_prgrm)
		return (0);
	av = (*prgrm)->s_astprgrm.argv;
	ac = (*prgrm)->s_astprgrm.ac;
	i = 0;
	while (i < ac)
		expand_arg(*prgrm, av[i++]);
	free_prgrm(*prgrm);
	*prgrm = n_prgrm;
	return (1);
}
