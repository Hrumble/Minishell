/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_attr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 17:23:28 by ngero             #+#    #+#             */
/*   Updated: 2026/11/28 18:36:49 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arena.h"
#include "lexer.h"

t_lexerattr	*new_lexerattr(char *buf, t_arena **arena)
{
	t_lexerattr	*attr;

	if (!buf)
		return (0);
	attr = arena_alloc(arena, sizeof(t_lexerattr), D_ALIGNMENT);
	if (!attr)
		return (0);
	attr->arena = arena;
	attr->buf = buf;
	attr->ptr = buf;
	attr->flags = 0;
	attr->ptr_index = 0;
	return (attr);
}
