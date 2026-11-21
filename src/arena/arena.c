/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <ngero@student.codam.nl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/21 14:00:12 by user              #+#    #+#             */
/*   Updated: 2026/11/21 14:37:16 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arena.h"
#include "libft.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static t_arena	*_arena_realloc(t_arena **a)
{
	t_arena	*new;

	if (!a || !*a)
		return (NULL);
	new = ft_realloc(*a, sizeof(t_arena) + ((*a)->capacity * 2));
	if (!new)
		return (NULL);
	new->capacity = new->capacity * 2;
	*a = new;
	return (new);
}

static char	*_get_next_aligned_address(t_arena *a, size_t alignment)
{
	uintptr_t	address;
	char 		*aligned_address;
	size_t		padding;

	if (!a)
		return (0);
	address = (uintptr_t)a->ptr; 
	padding = (alignment - (address % alignment)) % alignment;
	aligned_address = (char *)address + padding;
	return (aligned_address);
}

t_arena	*new_arena(size_t initial_capacity)
{
	t_arena	*a;

	a = malloc((sizeof(t_arena) + initial_capacity));
	if (!a)
		return (NULL);
	a->capacity = initial_capacity;
	a->ptr = (char *)a + sizeof(t_arena);
	return (a);
}

void	*arena_alloc(t_arena **a, size_t size, size_t alignment)
{
	char 	*aligned_address;

	if (!a || !*a)
		return (0);
	aligned_address = _get_next_aligned_address(*a, alignment);
	while ((char *)*a + sizeof(t_arena) + (*a)->capacity - aligned_address < size)
	{
		if (!_arena_realloc(a))
			return (0);
		aligned_address = _get_next_aligned_address(*a, alignment);
	}
	(*a)->ptr = aligned_address + size;
	return (aligned_address);
}
