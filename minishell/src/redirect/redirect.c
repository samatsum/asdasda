/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 14:10:23 by samatsum          #+#    #+#             */
/*   Updated: 2025/04/01 21:20:00 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "xlib.h"
#include "minishell.h"

void		do_redirect(t_node *redir, t_context *ctx);
void		reset_redirect(t_node *redir);
static bool	is_redirect(t_node *node);

/* ************************************************************************** */
void	do_redirect(t_node *redir, t_context *ctx)
{
	if (redir == NULL)
		return ;
	if (is_redirect(redir))
	{
		if (redir->from_fd == 0)
			redir->from_fd = openfd(redir, ctx);
		redir->stashed_out_fd = stashfd(redir->out_fd);
		xdup2(redir->from_fd, redir->out_fd);
	}
	else
		assert_error("do_redirect");
	do_redirect(redir->next, ctx);
}

/* ************************************************************************** */
void	reset_redirect(t_node *redir)
{
	if (redir == NULL)
		return ;
	reset_redirect(redir->next);
	if (is_redirect(redir))
	{
		xclose(redir->from_fd);
		xclose(redir->out_fd);
		xdup2(redir->stashed_out_fd, redir->out_fd);
	}
	else
		assert_error("reset_redirect");
}

/* ************************************************************************** */
static bool	is_redirect(t_node *node)
{
	if (node->kind == NODE_REDIR_OUT)
		return (true);
	else if (node->kind == NODE_REDIR_IN)
		return (true);
	else if (node->kind == NODE_REDIR_APPEND)
		return (true);
	else if (node->kind == NODE_REDIR_HEREDOC)
		return (true);
	return (false);
}
