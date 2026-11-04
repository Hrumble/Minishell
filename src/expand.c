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

static char	*expand_env(char **buf)
{
	int	i;
	char	env_name[20];
	char	*env;

	if (!buf || !*buf || **buf != '$')
		return (0);
	(*buf)++;
	i = 0;
	while (ft_isalnum(**buf))
		env_name[i++] = *(*buf)++;
	env_name[i] = 0;
	env = getenv(env_name);
	if (env)
		return (env);
	return ft_strdup("");
}

static void	expand_quotes(char **buf)
{
	char	*quote_end;

	if (**buf != '"' && **buf != '\'')
		return ;
	quote_end = find_matching_char(*buf);
	*quote_end = 0;
	(*buf)++;
}

static char	*expand_buf(char *buf)
{
	char	*env;
	size_t	expansion_size;
	size_t	i;
	char	*expanded;

	expansion_size = EXPANSION_SIZE;
	expanded = malloc(expansion_size * sizeof(char));
	if (!expanded)
		return (0);
	expand_quotes(&buf);
	i = 0;
	while (*buf)
	{
		env = expand_env(&buf);
		if (env)
		{
			if (!env_expansioncpy(&expanded, env, &i, &expansion_size))
				return (free(expanded), (void *)0);
			continue ;
		}
		if (!resize_expansion(&expanded, 1, i, &expansion_size))
			return (free(expanded), (void *)0);
		expanded[i++] = *buf++;
	}
	expanded[i] = 0;
	return (expanded);
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
