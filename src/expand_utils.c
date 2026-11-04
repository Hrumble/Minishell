/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 16:47:57 by ngero             #+#    #+#             */
/*   Updated: 2026/10/30 17:11:07 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "internal.h"
#include "parser.h"
#include <stddef.h>
#include <stdlib.h>

char	*find_matching_char(char *quote_pos)
{
	char	*sk;

	sk = quote_pos + 1;
	while (*sk && *sk != *quote_pos)
		sk++;
	if (!*sk)
		return (0);
	return (sk);
}

int	resize_expansion(char **expanded, size_t env_size, size_t i,
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

t_ast	*setup_expansion(t_ast **prgrm)
{
	t_ast	*n_prgrm;

	if (!prgrm || !*prgrm || (*prgrm)->type != AST_PRGRM)
		return (0);
	n_prgrm = new_astprgrm();
	if (!n_prgrm)
		return (0);
	n_prgrm->prgrm.redir_size = (*prgrm)->prgrm.redir_size;
	if (!prgrm_redircpy(n_prgrm->prgrm.redir, (*prgrm)->prgrm.redir,
			(*prgrm)->prgrm.redir_size))
		return (free(n_prgrm), (void *)0);
	return (n_prgrm);
}

int		env_expansioncpy(char **expanded, char *env, size_t *i,
		size_t *size)
{
	if (!resize_expansion(expanded, ft_strlen(env), *i, size))
		return (free(*expanded), 0);
	ft_memcpy(&(*expanded)[*i], env, ft_strlen(env));
	*i += ft_strlen(env);
	return (1);
}
