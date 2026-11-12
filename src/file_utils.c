/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <ngero@student.codam.nl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 09:55:57 by user              #+#    #+#             */
/*   Updated: 2026/11/12 10:01:37 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <unistd.h>

int	file_access(char *path, int mode)
{
	struct stat s;

	if (stat(path, &s))
		return (0);
	return (S_ISREG(s.st_mode) && !access(path, mode));
}
