/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <ngero@student.codam.nl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/21 14:00:22 by user              #+#    #+#             */
/*   Updated: 2026/11/28 18:58:52 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARENA_H
# define ARENA_H
# include <stddef.h>
# define D_ALIGNMENT 8

typedef struct s_arena {
	char	*ptr;
	char	*offset;
	size_t	capacity;
} t_arena ;

// Allocates a new arena with a capacity of initial capacity
t_arena	*new_arena(size_t initial_capacity);
// Mallocs something within the arena, uses alignment to properly align
// the requested allocation based on type.
void	*arena_alloc(t_arena **a, size_t size, size_t alignment);

// Libft rewrites

char	*ft_substr_arena(t_arena **a, const char *s, unsigned int start,
		size_t len);
char	*ft_strdup_arena(t_arena **a, const char *s);

#endif
