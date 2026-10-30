/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <ngero@student.codam.nl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 10:48:01 by user              #+#    #+#             */
/*   Updated: 2026/10/30 10:51:14 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

void	free_redir(t_prgrmredir *redir)
{
	if (!redir)
		return ;
	free(redir->target);
	free(redir);
}

void	free_prgrm(t_ast	*prgrm)
{
	int	i;

	if (!prgrm)
		return ;
	i = 0;
	while (i < prgrm->s_astprgrm.ac)
		free(prgrm->s_astprgrm.argv[i]);
	free(prgrm);
}
