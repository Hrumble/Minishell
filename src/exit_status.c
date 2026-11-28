/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <ngero@student.codam.nl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/28 18:15:36 by user              #+#    #+#             */
/*   Updated: 2026/11/28 18:17:12 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	*_get_exit_status()
{
	static int exit_status = 0;
	
	return (&exit_status);
}

int	get_exit_status()
{
	return (*_get_exit_status());
}

void	set_exit_status(int status)
{
	int	*exit_status;

	exit_status = _get_exit_status();
	*exit_status = status;
}
