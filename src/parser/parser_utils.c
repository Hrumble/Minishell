/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 22:05:16 by ngero             #+#    #+#             */
/*   Updated: 2026/11/10 11:26:40 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser.h"

int	is_empty_prgrm(t_ast *prgrm)
{
	if (!prgrm || prgrm->type != AST_PRGRM)
		return (0);
	return (prgrm->s_prgrm.ac == 0);
}

int	parse_redir(t_parserattr *pattr, t_lexerattr *lattr,
		t_ast *prgrm)
{
	t_token			*redir_tk;
	t_prgrmredir	**prgrm_redir;
	int				*prgrm_redir_size;

	prgrm_redir = prgrm->s_prgrm.redir;
	prgrm_redir_size = &prgrm->s_prgrm.redir_size;
	redir_tk = pattr->current;
	if (!consume_token(pattr, lattr))
		return (0);
	if (!peek(pattr, TK_WRD))
		return (0);
	prgrm_redir[(*prgrm_redir_size)++] = new_redir(redir_tk->redir_type,
			pattr->current->atom.ptr, pattr->current->atom.size);
	if (!prgrm_redir[*(prgrm_redir_size) - 1])
		return (0);
	if (!consume_token(pattr, lattr))
		return (0);
	return (1);
}

int	parse_word(t_parserattr *pattr, t_lexerattr *lattr, t_ast *prgrm)
{
	char	*substr;

	substr = ft_substr(pattr->current->atom.ptr, 0,
			pattr->current->atom.size);
	if (!substr)
		return (0);
	prgrm->s_prgrm.argv[prgrm->s_prgrm.ac] = substr;
	prgrm->s_prgrm.ac++;
	if (!consume_token(pattr, lattr))
		return (0);
	return (1);
}

int	prgrm_redircpy(t_prgrmredir **dest, t_prgrmredir **src, int redir_size)
{
	int				i;
	t_prgrmredir	*n_redir;

	i = 0;
	if (!src || !dest)
		return (0);
	while (i < redir_size)
	{
		n_redir = new_redir(src[i]->type,
				src[i]->target, ft_strlen(src[i]->target));
		if (!n_redir)
		{
			while (i--)
				free_redir(&dest[i]);
			return (0);
		}
		dest[i] = n_redir;
		i++;
	}
	return (1);
}

int	ast_map(t_ast	**ast, int (*f)(t_ast **ast))
{
	if (!ast)
		return (0);
	if ((*ast)->type == AST_BINARYOP)
	{
		if (!ast_map(&(*ast)->s_bop.left, f)
			|| !f(&(*ast)->s_bop.right))
			return (0);
		return (1);
	}
	if ((*ast)->type == AST_PRGRM)
		return (f(ast));
	return (1);
}
