/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/24 01:09:00 by ngero             #+#    #+#             */
/*   Updated: 2026/10/27 18:43:09 by ngero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TERMINAL_H
# define TERMINAL_H

// Sets the terminal to canonical or non-canonical mode
int		terminal_set_icanon(int icanon);
// sends a command to the terminal (\033 + command)
void	terminal_sequence(const char *command);
void	terminal_sequence_count(const char *command, int count);

#endif
