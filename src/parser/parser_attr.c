/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_attr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 20:34:51 by ngero             #+#    #+#             */
/*   Updated: 2026/10/29 20:43:49 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "parser.h"
#include <stdlib.h>

t_parserattr	*new_parserattr()
{
	t_parserattr	*attr;

	attr = malloc(sizeof(t_parserattr));
	if (!attr)
		return (0);
	attr->prev = 0;
	attr->current = 0;
	attr->next = 0;
	return (attr);
}
