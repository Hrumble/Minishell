/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 22:05:16 by ngero             #+#    #+#             */
/*   Updated: 2026/10/30 19:06:31 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser.h"

int	parse_redir(t_parserattr *pattr, t_lexerattr *lattr,
		t_ast *prgrm)
{
	t_token			*redir_tk;
	t_prgrmredir	**prgrm_redir;
	int				*prgrm_redir_size;

	prgrm_redir = prgrm->prgrm.redir;
	prgrm_redir_size = &prgrm->prgrm.redir_size;
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

void	ast_map(t_ast	**ast, void (*f)(t_ast **ast))
{
	if (!ast)
		return ;
	if ((*ast)->type == AST_BINARYOP)
	{
		ast_map(&(*ast)->bop.left, f);
		ast_map(&(*ast)->bop.right, f);
	}
	f(ast);
}
