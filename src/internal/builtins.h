/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 19:14:45 by ngero             #+#    #+#             */
/*   Updated: 2026/11/06 17:20:07 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

int	builtin_echo(char **av, int ac, char **env);
int	builtin_cd(char **av, int ac, char **env);
int	builtin_exit(char **av, int ac, char **env);
int	builtin_which(char **av, int ac, char **env);

#endif
