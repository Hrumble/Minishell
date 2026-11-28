/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_attr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 20:34:51 by ngero             #+#    #+#             */
/*   Updated: 2026/11/28 18:37:16 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arena.h"
#include "parser.h"

t_parserattr	*new_parserattr(t_arena **arena)
{
	t_parserattr	*attr;

	attr = arena_alloc(arena, sizeof(t_parserattr), D_ALIGNMENT);
	if (!attr)
		return (0);
	attr->arena = arena;
	attr->prev = 0;
	attr->current = 0;
	attr->next = 0;
	return (attr);
}
