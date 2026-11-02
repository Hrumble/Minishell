/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 16:17:38 by ngero             #+#    #+#             */
/*   Updated: 2026/11/01 22:25:56 by user             ###   ########.fr       */
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
	{
		path_var[end_ptr - buf - 1] = *end_ptr;
		end_ptr++;
	}
	path_var[end_ptr - buf - 1] = 0;
	var = getenv(path_var);
	if (!var)
		return (ft_strdup(""));
	return (var);
}

static int	expand_arg(t_ast *prgrm, char *buf)
{
	int		is_in_squotes;
	char	*quote_end;

	is_in_squotes = 0;
	if (*buf == '\'' || *buf == '"')
	{
		quote_end = find_matching_char(buf);
		if (quote_end)
			return (0);
		*quote_end = 0;
		if (*buf == '\'')
			is_in_squotes = 1;
		buf++;
	}
	if (*buf == '$' && !is_in_squotes)
		prgrm->prgrm.argv[prgrm->prgrm.ac++] = expand_env(buf);
	else
		prgrm->prgrm.argv[prgrm->prgrm.ac++] = ft_strdup(buf);
	return (1);
}

int	expand_argv(t_ast	**prgrm)
{
	int		ac;
	char	**av;
	int		i;
	t_ast	*n_prgrm;

	if (!prgrm || !*prgrm || (*prgrm)->type != AST_PRGRM)
		return (0);
	n_prgrm = new_astprgrm();
	if (!n_prgrm)
		return (0);
	av = (*prgrm)->prgrm.argv;
	ac = (*prgrm)->prgrm.ac;
	n_prgrm->prgrm.redir_size = (*prgrm)->prgrm.redir_size;
	if (!prgrm_redircpy(n_prgrm->prgrm.redir, (*prgrm)->prgrm.redir,
			(*prgrm)->prgrm.redir_size))
		return (free(n_prgrm), 0);
	i = 0;
	while (i < ac)
		if (!expand_arg(n_prgrm, av[i++]))
			return (free(n_prgrm), 0);
	free_prgrm(*prgrm);
	*prgrm = n_prgrm;
	return (1);
}
