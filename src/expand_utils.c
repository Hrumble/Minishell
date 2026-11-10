/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 16:47:57 by ngero             #+#    #+#             */
/*   Updated: 2026/11/10 22:23:49 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "internal.h"
#include "parser.h"
#include <stddef.h>
#include <stdlib.h>

int	expand_assign_character(t_bufexpand *ex, char c, size_t i)
{
	if (!resize_expansion(ex, 1, i))
		return (0);
	ex->expanded[i] = c;
	return (1);
}

int	resize_expansion(t_bufexpand *ex, size_t env_size, size_t i)
{
	char	*new;

	while (env_size > (ex->exsize - i))
	{
		new = ft_realloc(ex->expanded, ex->exsize);
		if (!new)
			return (0);
		ex->exsize = ex->exsize * 2;
		ex->expanded = new;
	}
	return (1);
}

t_ast	*setup_expansion(t_ast **prgrm)
{
	t_ast	*n_prgrm;

	if (!prgrm || !*prgrm || (*prgrm)->type != AST_PRGRM)
		return (0);
	n_prgrm = new_astprgrm();
	if (!n_prgrm)
		return (0);
	n_prgrm->s_prgrm.redir_size = (*prgrm)->s_prgrm.redir_size;
	if (!prgrm_redircpy(n_prgrm->s_prgrm.redir, (*prgrm)->s_prgrm.redir,
			(*prgrm)->s_prgrm.redir_size))
		return (free(n_prgrm), (void *)0);
	return (n_prgrm);
}

int	setup_buf_expansion(t_bufexpand *ex, size_t *i, char *buf)
{
	if (!buf)
		return (0);
	ex->exsize = ft_strlen(buf);
	ex->flags = 0;
	ex->buf = buf;
	ex->expanded = malloc(ex->exsize * sizeof(char));
	*i = 0;
	if (!ex->expanded)
		return (0);
	return (1);
}

int	env_expansioncpy(t_bufexpand *ex, char *env, size_t *i)
{
	size_t	env_len;

	if (!env)
		return (0);
	env_len = ft_strlen(env);
	if (!resize_expansion(ex, env_len, *i))
		return (0);
	ft_memcpy(&(ex->expanded[*i]), env, env_len);
	*i += env_len;
	return (1);
}
