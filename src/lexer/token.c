/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 18:04:06 by ngero             #+#    #+#             */
/*   Updated: 2026/10/30 10:56:28 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stddef.h>
#include <stdlib.h>

t_token	*new_token(t_tokentype type, char *ptr, size_t size)
{
	t_token	*t;

	t = malloc(sizeof(t_token));
	if (!t)
		return (0);
	t->type = type;
	if (ptr)
		t->atom = (t_atom){ptr, size};
	return (t);
}

t_token	*new_token_redir(t_redirtype type)
{
	t_token	*t;

	t = new_token(TK_REDIR, 0, 0);
	if (!t)
		return (0);
	t->redir_type = type;
	return (t);
}
