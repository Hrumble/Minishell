/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 20:44:34 by ngero             #+#    #+#             */
/*   Updated: 2026/10/29 20:59:02 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parser.h"
#include <stdlib.h>

t_ast	*parse_program(t_parserattr *pattr, t_lexerattr *lattr)
{
	t_ast	*prgrm;

	if (!peek(pattr, TK_WRD) || pattr->prev)
		return (0);
	prgrm = malloc(sizeof(t_ast));
	if (!prgrm)
		return (0);
	prgrm->type = AST_PRGRM;
	consume_token(pattr, lattr);
	while (peek(pattr, TK_WRD))
}

t_ast	*parse_ast(char	*buf)
{
	t_parserattr	*pattr;
	t_lexerattr		*lattr;
	t_ast			*node;

	pattr = new_parserattr();
	lattr = new_lexerattr(buf);
	if (!pattr || !lattr)
		return (free(pattr), free(lattr), NULL);
	consume_token(pattr, lattr);
	while (pattr->current)
	{
		node = parse_program(pattr, lattr);
		if (node)
	}
}
