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

static int	resize_expansion(char **expanded, size_t env_size, size_t i,
		size_t *size)
{
	char	*new;
	if (env_size < (*size - i))
		return (1);
	new = ft_realloc(*expanded, *size);
	if (!new)
		return (0);
	*size = *size * 2;
	*expanded = new;
	return (1);
}

static int	expand_arg(t_ast *prgrm, char *buf)
{
	char	*env;
	size_t	expansion_size;
	size_t	i;
	char	*expanded;

	expansion_size = EXPANSION_SIZE;
	expanded = malloc(expansion_size * sizeof(char));
	if (!expanded)
		return (0);
	i = 0;
	while (*buf)
	{
		env = expand_env(&buf);
		if (env)
		{
			if (!resize_expansion(&expanded, ft_strlen(env), i, &expansion_size))
				return (free(expanded), 0);
			ft_memcpy(&expanded[i], env, ft_strlen(env));
			i += ft_strlen(env);
			continue ;
		}
		if (!resize_expansion(&expanded, 1, i, &expansion_size))
			return (free(expanded), 0);
		expanded[i++] = *buf++;
	}
	expanded[i] = 0;
	prgrm->prgrm.argv[prgrm->prgrm.ac++] = expanded;
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
