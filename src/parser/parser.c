/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 20:02:49 by ngero             #+#    #+#             */
/*   Updated: 2026/10/29 22:54:39 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "lexer.h"

t_token	*consume_token(t_parserattr *pattr, t_lexerattr *lattr)
{
	t_token	*tk;

	if (!pattr->current)
	{
		pattr->current = next_token(lattr);
		pattr->next = next_token(lattr);
		return (pattr->current);
	}
	tk = pattr->current;
	pattr->prev = pattr->current;
	pattr->current = pattr->next;
	pattr->next = next_token(lattr);
	return (tk);
}

int	peek(t_parserattr *pattr, t_tokentype type)
{
	if (!pattr || !pattr->current)
		return (0);
	return (pattr->current->type == type);
}

