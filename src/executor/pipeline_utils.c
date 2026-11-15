/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <ngero@student.codam.nl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/15 12:11:04 by user              #+#    #+#             */
/*   Updated: 2026/11/15 17:16:56 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

int	count_pipeline_programs(t_ast *pipeline)
{
	if (pipeline->type == AST_BINARYOP)
	{
		return (count_pipeline_programs(pipeline->s_bop.left)
		+ count_pipeline_programs(pipeline->s_bop.right));
	}
	return (1);
}

t_ast	**flatten_pipeline(t_ast *pipeline)
{
	t_ast	**programs;
	t_ast	*current;
	int		i;
	int		size;

	size = count_pipeline_programs(pipeline);
	programs = malloc((size + 1) * sizeof(t_ast *));
	if (!programs)
		return (0);
	programs[size] = 0;
	i = size - 1;
	current = pipeline;
	while (current->type == AST_BINARYOP)
	{
		programs[i--] = current->s_bop.right;
		current = current->s_bop.left;
	}
	programs[i] = current;
	return (programs);
}
