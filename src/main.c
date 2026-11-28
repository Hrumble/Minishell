/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 14:53:48 by ngero             #+#    #+#             */
/*   Updated: 2026/11/28 18:18:53 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal.h"
#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	(void)av;
	(void)ac;
	minishell(env);
	return (get_exit_status());
}
