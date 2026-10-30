/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 20:44:34 by ngero             #+#    #+#             */
/*   Updated: 2026/10/30 12:19:56 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "parser.h"
#include <stdlib.h>

static int	parse_redir(t_parserattr *pattr, t_lexerattr *lattr,
		t_ast *prgrm)
{
	t_token			*redir_tk;
	t_prgrmredir	**prgrm_redir;
	int				*prgrm_redir_size;

	prgrm_redir = prgrm->s_astprgrm.redir;
	prgrm_redir_size = &prgrm->s_astprgrm.redir_size;
	if (!peek(pattr, TK_REDIR))
		return (0);
	redir_tk = pattr->current;
	if (!consume_token(pattr, lattr))
		return (-1);
	if (!peek(pattr, TK_WRD))
		return (-1);
	prgrm_redir[(*prgrm_redir_size)++] = new_redir(redir_tk->redir_type,
			pattr->current->atom.ptr, pattr->current->atom.size);
	if (prgrm_redir[*(prgrm_redir_size) - 1] != 0)
		return (1);
	return (-1);
}

static t_ast	*parse_program(t_parserattr *pattr, t_lexerattr *lattr)
{
	char	*substr;
	int		ps_redir_out;
	t_ast	*prgrm;

	prgrm = new_astprgrm();
	if (!prgrm)
		return (0);
	while (peek(pattr, TK_WRD) || peek(pattr, TK_REDIR))
	{
		ps_redir_out = parse_redir(pattr, lattr, prgrm);
		if (ps_redir_out == -1)
			return (free_prgrm(prgrm), (void *)0);
		else if (ps_redir_out && consume_token(pattr, lattr))
			continue ;
		substr = ft_substr(pattr->current->atom.ptr, 0,
				pattr->current->atom.size);
		if (!substr)
			return (free_prgrm(prgrm), (void *)0);
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
	if (!pattr || !lattr || !consume_token(pattr, lattr))
		return (free(pattr), free(lattr), NULL);
	return (parse_program(pattr, lattr));
}
