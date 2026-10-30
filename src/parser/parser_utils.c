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

#include "parser.h"

int	parse_redir(t_parserattr *pattr, t_lexerattr *lattr,
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
