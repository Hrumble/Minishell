/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <ngero@student.codam.nl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 10:48:01 by user              #+#    #+#             */
/*   Updated: 2026/11/21 16:04:32 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parser.h"
#include <stdlib.h>

void	free_ast(t_ast **ast)
{
	if (!ast || !*ast)
		return ;
	if ((*ast)->type == AST_BINARYOP)
	{
		free_prgrm(&(*ast)->s_bop.right);
		free_ast(&(*ast)->s_bop.left);
		free(*ast);
		*ast = 0;
		return ;
	}
	free_prgrm(ast);
	*ast = 0;
}

void	free_redir(t_prgrmredir **redir)
{
	if (!redir || !*redir)
		return ;
	free((*redir)->target);
	free((*redir));
	*redir = 0;
}

void	free_prgrm(t_ast	**prgrm)
{
	int	i;

	if (!prgrm || !*prgrm)
		return ;
	i = 0;
	while (i < (*prgrm)->s_prgrm.ac)
		free((*prgrm)->s_prgrm.argv[i++]);
	i = 0;
	while (i < (*prgrm)->s_prgrm.redir_size)
		free_redir(&(*prgrm)->s_prgrm.redir[i++]);
	free(*prgrm);
	*prgrm = 0;
}

void	free_pandlattr(t_parserattr **pattr, t_lexerattr **lattr)
{
	if (pattr && *pattr)
	{
		free((*pattr)->prev);
		free((*pattr)->current);
		free((*pattr)->next);
		free(*pattr);
		*pattr = 0;
	}
	if (lattr && *lattr)
	{
		// free((*lattr)->buf);
		free(*lattr);
		*lattr = 0;
	}
}
