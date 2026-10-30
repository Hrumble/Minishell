/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_nodes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 20:55:02 by ngero             #+#    #+#             */
/*   Updated: 2026/10/30 13:08:32 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser.h"
#include <stddef.h>
#include <stdlib.h>

t_ast	*new_astbinaryop(t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (0);
	node->type = AST_BINARYOP;
	node->s_binaryop.binaryop = BOP_PIPE;
	node->s_binaryop.left = left;
	node->s_binaryop.right = right;
	return (node);
}

t_ast	*new_astprgrm()
{
	t_ast	*prgrm;

	prgrm = malloc(sizeof(t_ast));
	if (!prgrm)
		return (0);
	prgrm->type = AST_PRGRM;
	prgrm->s_astprgrm.ac = 0;
	prgrm->s_astprgrm.redir_size = 0;
	return (prgrm);
}

t_prgrmredir	*new_redir(t_redirtype type, char *buf, size_t len)
{
	t_prgrmredir	*redir;

	redir = malloc(sizeof(t_prgrmredir));
	if (!redir)
		return (0);
	redir->target = ft_substr(buf, 0, len);
	if (!redir->target)
		return (free(redir), (void *)0);
	redir->type = type;
	return (redir);
}
