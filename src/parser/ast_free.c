/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <ngero@student.codam.nl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 10:48:01 by user              #+#    #+#             */
/*   Updated: 2026/11/05 14:46:11 by ngero            ###   ########.fr       */
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
	while (i < prgrm->s_prgrm.ac)
		free(prgrm->s_prgrm.argv[i++]);
	i = 0;
	while (i < prgrm->s_prgrm.redir_size)
		free(prgrm->s_prgrm.redir[i++]);
	free(prgrm);
}
