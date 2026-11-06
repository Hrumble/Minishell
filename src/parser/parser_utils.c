/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 22:05:16 by ngero             #+#    #+#             */
/*   Updated: 2026/11/05 14:43:50 by ngero            ###   ########.fr       */
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
				free_redir(dest[i]);
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
			|| !ast_map(&(*ast)->s_bop.right, f))
			return (0);
		return (1);
	}
	return (f(ast));
}
