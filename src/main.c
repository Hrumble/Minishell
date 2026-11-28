/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 14:53:48 by ngero             #+#    #+#             */
/*   Updated: 2026/11/21 15:46:25 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	g_exit_status;

int	main(int ac, char **av, char **env)
{
	(void)av;
	(void)ac;
	minishell(env);
	return (g_exit_status);
}
