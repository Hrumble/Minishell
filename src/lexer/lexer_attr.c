/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_attr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 17:23:28 by ngero             #+#    #+#             */
/*   Updated: 2026/10/29 17:24:37 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stdlib.h>

t_lexerattr *new_lexerattr(char *buf)
{
	t_lexerattr	*attr;

	if (!buf)
		return (0);
	attr = malloc(sizeof(t_lexerattr));
	if (!attr)
		return (0);
	attr->buf = buf;
	attr->ptr = buf;
	attr->flags = 0;
	return (attr);
}

