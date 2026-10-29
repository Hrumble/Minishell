/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 20:44:34 by ngero             #+#    #+#             */
/*   Updated: 2026/10/29 22:56:28 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "parser.h"
#include <stdlib.h>

static t_ast	*parse_program(t_parserattr *pattr, t_lexerattr *lattr)
{
	char	*substr;
	t_ast	*prgrm;

	prgrm = new_astprgrm();
	if (!prgrm)
		return (0);
	consume_token(pattr, lattr);
	while (peek(pattr, TK_WRD))
	{
		substr = ft_substr(pattr->current->atom.ptr, 0,
				pattr->current->atom.size);
		if (!substr)
			return (free(prgrm), (void *)0);
		prgrm->s_astprgrm.argv[prgrm->s_astprgrm.ac] = substr;
		prgrm->s_astprgrm.ac++;
		consume_token(pattr, lattr);
	}
	return (prgrm);
}

t_ast	*parse_ast(char	*buf)
{
	t_parserattr	*pattr;
	t_lexerattr		*lattr;

	pattr = new_parserattr();
	lattr = new_lexerattr(buf);
	if (!pattr || !lattr)
		return (free(pattr), free(lattr), NULL);
	return (parse_program(pattr, lattr));
}
