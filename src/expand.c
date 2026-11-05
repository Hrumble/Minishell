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

#include "internal.h"
#include "libft.h"
#include "parser.h"
#include <stddef.h>
#include <stdlib.h>

int	expand_env(char **dest, char **buf)
{
	int		i;
	char	env_name[20];
	char	*env;

	if (!buf || !*buf || **buf != '$' || !ft_isalnum(*(*buf + 1)))
		return (0);
	(*buf)++;
	i = 0;
	while (ft_isalnum(**buf))
		env_name[i++] = *(*buf)++;
	env_name[i] = 0;
	env = getenv(env_name);
	if (env)
		*dest = env;
	else
		*dest = ft_strdup("");
	return (1);
}

static int	expand_quotes(t_bufexpand *ex)
{
	char	*quote_end;

	ex->in_squotes = 0;
	if (*ex->buf != '"' && *ex->buf != '\'')
		return (1);
	if (*ex->buf == '\'')
		ex->in_squotes = 1;
	quote_end = find_matching_char(ex->buf);
	if (!quote_end)
		return (0);
	*quote_end = 0;
	(ex->buf)++;
	return (1);
}

static char	*expand_buf(char *buf)
{
	char		*env;
	t_bufexpand	ex;
	size_t		i;

	setup_buf_expansion(&ex, &i, buf);
	if (!expand_quotes(&ex))
		return (0);
	while (*ex.buf)
	{
		if (!ex.in_squotes && expand_env(&env, &ex.buf))
		{
			if (!env_expansioncpy(&ex, env, &i))
				return (free(ex.expanded), (void *)0);
			continue ;
		}
		if (!resize_expansion(&ex, 1, i))
			return (free(ex.expanded), (void *)0);
		ex.expanded[i++] = *ex.buf++;
	}
	ex.expanded[i] = 0;
	return (ex.expanded);
}

int	expand_argv(t_ast	**prgrm)
{
	int		ac;
	char	**av;
	int		i;
	t_ast	*n_prgrm;

	n_prgrm = setup_expansion(prgrm);
	if (!n_prgrm)
		return (0);
	av = (*prgrm)->prgrm.argv;
	ac = (*prgrm)->prgrm.ac;
	i = 0;
	while (i < ac)
	{
		n_prgrm->prgrm.argv[n_prgrm->prgrm.ac] = expand_buf(av[i++]);
		if (!n_prgrm->prgrm.argv[n_prgrm->prgrm.ac++])
			return (free(n_prgrm), 0);
	}
	free_prgrm(*prgrm);
	*prgrm = n_prgrm;
	return (1);
}

int	expand_redir(t_ast	**prgrm)
{
	char	*expanded;
	int		i;

	i = 0;
	while (i < (*prgrm)->prgrm.redir_size)
	{
		expanded = expand_buf((*prgrm)->prgrm.redir[i]->target);
		if (!expanded)
			return (0);
		free((*prgrm)->prgrm.redir[i]->target);
		(*prgrm)->prgrm.redir[i]->target = expanded;
		i++;
	}
	return (1);
}
