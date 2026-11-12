/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngero <ngero@student.codam.nl>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 16:02:26 by ngero             #+#    #+#             */
/*   Updated: 2026/11/12 10:35:54 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H
#include <sys/types.h>
# define PATH_SIZE 100
# define MAX_PROCESSES 100
# define HEREDOC_FILE_PATH "/tmp/mshell_heredoc"

#include "parser.h"

typedef struct s_defaultredir
{
	int	default_io[2];
}	t_defaultredir;

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
int		set_redirection_stream(t_prgrmredir *redir, t_defaultredir *credirs);
// Handles the heredocs redir
int		handle_heredocs_redir(char *delim, t_defaultredir *credirs);
// sets up the redirections for a program.
// if builtin is set to 1, kbfd and termfd will also be set within the struct
int		setup_prgrm_redirections(t_ast *ast, t_defaultredir *credirs);
// Restores the default 0 and 1 streams from the credirs struct
int		restore_default_streams(t_defaultredir *credirs);
// Saves the current 0 and 1 streams into the current redirs struct
int		save_default_streams(t_defaultredir *credirs);
// Pipes the specified file descriptor to the file specified on path
// returns the fd of the newly openened file.
// The mode is used to specify the mode in which to open the file.
// O_CREAT should be default
int		pipe_fd_to_file(int fd, char *path, int mode);
// Returns a function pointer to the builtin function which the specified name.
// 0 if none
int (*get_builtin(char *buf))(char **, int, char **);
// Executes a builtin shell command
int		exec_builtin(t_ast *prgrm, char **env, int fd[2]);
// Executes an entire pipeline. To be called on BOPs
int		execute_pipeline(t_ast	*pipeline, char **env);
int		setup_pipeline_redirections(int fd[2]);
// awaits every process in the list one by one
void	await_processes(pid_t *processes, int size);

// freeing stuff
void	free_split(char **split);

#endif
