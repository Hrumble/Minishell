/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 16:02:26 by ngero             #+#    #+#             */
/*   Updated: 2026/11/08 18:03:45 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H
#include <sys/types.h>
# define PATH_SIZE 100
# define MAX_PROCESSES 100

#include "parser.h"

typedef struct s_currentredirs
{
	int	in;
	int	out;
	int	kbfd;
	int	termfd;
}	t_currentredirs;

// Executes an ast, returns the pid of the fork on success
// 0 on failure
int		fork_and_exec(t_ast *ast, char **env, int fd[2]);

// Finds a binary from relative path or PATH
// returns (void *)0 on failure to find.
char	*find_binary(char *buf);
// Runs a builtin function, providing it **av and ac. Sets the status.
// always returns (1)
int		run_builtin(int (*builtin)(char **, int, char **), t_ast *prgrm,
			char **env, int fd[2]);

// Redirections

// sets up the read and write entries specified by a redir.
int		pipe_redirection(t_prgrmredir *redir);
// sets up the redirections for a program.
// if builtin is set to 1, kbfd and termfd will also be set within the struct
int		setup_prgrm_redirections(t_ast *ast, t_currentredirs *credirs, int builtin);
// Restores the default 0 and 1 streams.
int		restore_inout(t_currentredirs *credirs);
// Pipes the specified file descriptor to the file specified on path
// returns the fd of the newly openened file.
// The mode is used to specify the mode in which to open the file.
// O_CREAT should be default
int		pipe_fd_to_file(int fd, char *path, int mode);
// Executes a builtin shell command
int		_exec_builtin(t_ast *prgrm, char **env, int fd[2]);
// Executes an entire pipeline. To be called on BOPs
int		execute_pipeline(t_ast	*pipeline, char **env);
int		setup_pipeline_redirections(int fd[2]);
// awaits every process in the list one by one
void	await_processes(pid_t *processes, int size);

// freeing stuff
void	free_split(char **split);

#endif
