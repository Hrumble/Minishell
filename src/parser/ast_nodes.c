/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_nodes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 20:55:02 by ngero             #+#    #+#             */
/*   Updated: 2026/10/29 22:32:01 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

t_ast	*new_astprgrm()
{
	t_ast	*prgrm;

	prgrm = malloc(sizeof(t_ast));
	if (!prgrm)
		return (0);
	prgrm->type = AST_PRGRM;
	prgrm->s_astprgrm.ac = 0;
	return (prgrm);
}
