/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <ngero@student.codam.nl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/21 14:49:41 by user              #+#    #+#             */
/*   Updated: 2026/11/21 14:51:44 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arena.h"
#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

char	*ft_strdup_arena(t_arena **a, const char *s)
{
	char		*sdup;
	size_t		len;

	len = ft_strlen(s);
	sdup = (char *)arena_alloc(a, (len + 1) * sizeof(char), D_ALIGNMENT);
	if (!sdup)
		return (0);
	ft_memcpy(sdup, s, len * sizeof(char));
	*(sdup + len) = 0;
	return (sdup);
}

char	*ft_substr_arena(t_arena **a, const char *s, unsigned int start,
		size_t len)
{
	size_t		real_len;
	char		*substr;

	if (!s)
		return (0);
	if (start > ft_strlen(s))
		return (ft_strdup_arena(a, ""));
	real_len = ft_strlen(s + start);
	if (len > real_len)
		len = real_len;
	substr = arena_alloc(a, (len + 1) * sizeof(char), D_ALIGNMENT);
	if (!substr)
		return (0);
	if (!substr)
		return (0);
	ft_memcpy(substr, s + start, len);
	*(substr + len) = 0;
	return (substr);
}
