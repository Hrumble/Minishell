/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 16:17:38 by ngero             #+#    #+#             */
/*   Updated: 2026/11/10 22:40:02 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"
#include "libft.h"
#include "parser.h"
#include <stddef.h>
#include <stdlib.h>

int	handle_env_expansion(t_bufexpand *ex, size_t *i)
{
	int		j;
	char	env_name[50];
	char	*env;
	int		ok;

	if (*ex->buf != '$' || !ft_isalnum(ex->buf[1]))
		return (0);
	ex->buf++;
	j = 0;
	while (ft_isalnum(*ex->buf))
		env_name[j++] = *ex->buf++;
	env_name[j] = 0;
	env = vtable_get(env_name);
	if (env)
		ok = env_expansioncpy(ex, env, i);
	else
	{
		env = ft_strdup("");
		ok = env_expansioncpy(ex, env, i);
	}
	free(env);
	return (ok);
}

void	handle_quote_expansion(t_bufexpand *ex, size_t *i)
{
	while (ft_isquotes(*ex->buf))
	{
		if (!get_bit(ex->flags, 0) && !get_bit(ex->flags, 1))
		{
			if (*ex->buf == '"')
				set_bit(&ex->flags, 0);
			if (*ex->buf == '\'')
				set_bit(&ex->flags, 1);
			ex->buf++;
			continue ; 
		}
		if (get_bit(ex->flags, 0) && *ex->buf == '"')
		{
			clear_bit(&ex->flags, 0);
			ex->buf++;
			continue ;
		}
		if (get_bit(ex->flags, 1) && *ex->buf == '\'')
		{
			clear_bit(&ex->flags, 1);
			ex->buf++;
			continue ;
		}
		expand_assign_character(ex, *ex->buf++, (*i)++);
	}
}

static char	*expand_buf(char *buf)
{
	t_bufexpand	ex;
	size_t		i;

	// "hello""world"
	if (!setup_buf_expansion(&ex, &i, buf))
		return (0);
	while (*ex.buf)
	{
		if (ft_isquotes(*ex.buf))
			handle_quote_expansion(&ex, &i);
		if (*ex.buf == '$' && !get_bit(ex.flags, 1))
			if (!handle_env_expansion(&ex, &i))
				return (free(ex.expanded), NULL);
		if (!*ex.buf)
			break ;
		expand_assign_character(&ex, *ex.buf++, i++);
	}
	expand_assign_character(&ex, 0, i);
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
	av = (*prgrm)->s_prgrm.argv;
	ac = (*prgrm)->s_prgrm.ac;
	i = 0;
	while (i < ac)
	{
		n_prgrm->s_prgrm.argv[n_prgrm->s_prgrm.ac] = expand_buf(av[i++]);
		if (!n_prgrm->s_prgrm.argv[n_prgrm->s_prgrm.ac++])
			return (free_prgrm(&n_prgrm), 0);
	}
	free_prgrm(prgrm);
	*prgrm = n_prgrm;
	return (1);
}

int	expand_redir(t_ast	**prgrm)
{
	char	*expanded;
	int		i;

	i = 0;
	while (i < (*prgrm)->s_prgrm.redir_size)
	{
		expanded = expand_buf((*prgrm)->s_prgrm.redir[i]->target);
		if (!expanded)
			return (0);
		free((*prgrm)->s_prgrm.redir[i]->target);
		(*prgrm)->s_prgrm.redir[i]->target = expanded;
		i++;
	}
	return (1);
}
