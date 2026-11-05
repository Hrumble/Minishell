/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 17:09:06 by ngero             #+#    #+#             */
/*   Updated: 2026/11/05 17:16:04 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <stdlib.h>

void	free_split(char **split)
{
	char	**seeker;

	if (!split)
		return ;
	seeker = split;
	while (*seeker)
		free(*seeker++);
	free(split);
}
