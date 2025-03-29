/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:25:13 by samatsum          #+#    #+#             */
/*   Updated: 2025/03/29 16:32:06 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include "minishell.h"

int				exec(t_node *node, t_context *ctx);
static pid_t	exec_pipeline(t_node *node, t_context *ctx);
static void		exec_nonbuiltin(t_node *node, t_context *ctx)
				__attribute__((noreturn));
static void		validate_access(const char *path, const char *filename_token, \
	const char *tmp);

int	exec(t_node *node, t_context *ctx)
{
	pid_t	last_pid;
	int		status;

	open_redir_file(node, ctx);
	if (node->next == NULL && is_builtin(node))
		status = exec_builtin(node, ctx);
	else
	{
		last_pid = exec_pipeline(node, ctx);
		wait_pipeline(last_pid, &status);
	}
	return (status);
}

/* ************************************************************************** */
static pid_t	exec_pipeline(t_node *node, t_context *ctx)
{
	pid_t		pid;

	if (node == NULL)
		return (-1);
	if (!node->cmd_node->args_token->word && node->cmd_node->args_token->next)
		node->cmd_node->args_token = node->cmd_node->args_token->next;
	prepare_pipe(node);
	pid = fork();
	if (pid < 0)
		fatal_error("fork");
	else if (pid == 0)
	{
		reset_signal();
		prepare_pipe_child(node);
		if (is_builtin(node))
			exit(exec_builtin(node, ctx));
		else
			exec_nonbuiltin(node, ctx);
	}
	prepare_pipe_parent(node);
	if (node->next)
		return (exec_pipeline(node->next, ctx));
	return (pid);
}

/* ************************************************************************** */
static void	exec_nonbuiltin(t_node *node, t_context *ctx)
{
	char		*path;
	char		**argv;

	do_redirect(node->cmd_node->redirects_node, ctx);
	argv = token_list_to_argv(node->cmd_node->args_token);
	path = argv[0];
	if (path != NULL)
		if (ft_strchr(path, '/') == NULL)
			path = search_path(path, ctx);
	validate_access(path, argv[0], node->cmd_node->args_token->word);
	execve(path, argv, get_environ(ctx->envmap));
	free_argv(argv);
	reset_redirect(node->cmd_node->redirects_node);
	if (path == NULL)
	{
		reset_redirect(node->cmd_node->redirects_node);
		exit(0);
	}
	fatal_error("execve");
}

/* ************************************************************************** */
static void	validate_access(const char *path, const char *filename_token, \
	const char *tmp)
{
	struct stat	st;

	if (path == NULL)
	{
		if (tmp != NULL)
			err_exit(filename_token, "command not found", NOT_FOUND);
		else
			exit(0);
	}
	if (ft_strcmp(filename_token, "") == 0)
		err_exit(filename_token, "command not found", NOT_FOUND);
	if (ft_strcmp(filename_token, "..") == 0)
		err_exit(filename_token, "command not found", NOT_FOUND);
	if (access(path, F_OK) < 0)
		err_exit(filename_token, "No such file or directory", NOT_FOUND);
	if (stat(path, &st) < 0)
		fatal_error("fstat");
	if (S_ISDIR(st.st_mode))
		err_exit(filename_token, "Is a directory", PERMISSION_DENIED);
	if (access(path, X_OK) < 0)
		err_exit(path, "Permission denied", PERMISSION_DENIED);
}
