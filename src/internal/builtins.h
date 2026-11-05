/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 19:14:45 by ngero             #+#    #+#             */
/*   Updated: 2026/11/05 14:42:54 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

int	builtin_echo(char **av, int ac);
int	builtin_cd(char **av, int ac);
int	builtin_exit(char **av, int ac);

#endif
