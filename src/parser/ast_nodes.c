/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_nodes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 20:55:02 by ngero             #+#    #+#             */
/*   Updated: 2026/11/05 14:45:08 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
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
	node->s_bop.binaryop = BOP_PIPE;
	node->s_bop.left = left;
	node->s_bop.right = right;
	return (node);
}

t_ast	*new_astprgrm(void)
{
	t_ast	*prgrm;

	prgrm = malloc(sizeof(t_ast));
	if (!prgrm)
		return (0);
	prgrm->type = AST_PRGRM;
	prgrm->s_prgrm.ac = 0;
	prgrm->s_prgrm.redir_size = 0;
	return (prgrm);
}

t_ast	*new_astdef(t_defatom *datom)
{
	char	*keydup;
	char	*valdup;
	t_ast	*def;

	def = malloc(sizeof(t_ast));
	if (!def)
		return (0);
	keydup = ft_substr(datom->key.ptr, 0, datom->key.size);
	valdup = ft_substr(datom->val.ptr, 0, datom->val.size);
	if (!keydup || !valdup)
		return (free(keydup), free(valdup), (void *)0);
	def->type = AST_DEF;
	def->s_def.key = keydup;
	def->s_def.value = valdup;
	return (def);
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
