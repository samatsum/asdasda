/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:21:12 by samatsum          #+#    #+#             */
/*   Updated: 2025/03/29 17:27:55 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include "ft_printf.h"
#include "xlib.h"
#include "minishell.h"

int			read_heredoc(const char *delimiter_token, bool is_delim_unquoted, \
	t_context *ctx);
static void	readline_heredoc_loop(int pfd[2], const char *delimiter_token, \
	bool is_delim_unquoted, t_context *ctx);
static bool	process_heredoc_line(char *line, const char *delimiter_token);
static char	*expand_heredoc_line(char *line, t_context *ctx);

/* ************************************************************************** */
int	read_heredoc(const char *delimiter_token, bool is_delim_unquoted, \
	t_context *ctx)
{
	int		pfd[2];

	xpipe(pfd);
	readline_heredoc_loop(pfd, delimiter_token, is_delim_unquoted, ctx);
	xclose(pfd[1]);
	if (g_signal_received == SIGINT)
	{
		xclose(pfd[0]);
		return (-1);
	}
	return (pfd[0]);
}

/* ************************************************************************** */
static void	readline_heredoc_loop(int pfd[2], const char *delimiter_token, \
	bool is_delim_unquoted, t_context *ctx)
{
	char	*line;
	bool	should_break;

	while (1)
	{
		line = readline("> ");
		should_break = process_heredoc_line(line, delimiter_token);
		if (should_break)
			break ;
		if (is_delim_unquoted)
			line = expand_heredoc_line(line, ctx);
		ft_dprintf(pfd[1], "%s\n", line);
		free(line);
		line = NULL;
	}
}

static bool	process_heredoc_line(char *line, const char *delimiter_token)
{
	if (line == NULL || g_signal_received == SIGINT)
	{
		if (line == NULL && delimiter_token != NULL)
		{
			perror_prefix();
			ft_dprintf(STDERR_FILENO,
				"warning: here-document at line 1 delimited \
				by end-of-file (wanted `%s')\n", delimiter_token);
		}
		free(line);
		return (true);
	}
	if (ft_strcmp(line, delimiter_token) == 0)
	{
		free(line);
		return (true);
	}
	return (false);
}

/* ************************************************************************** */
static char	*expand_heredoc_line(char *line, t_context *ctx)
{
	char	*new_word;
	char	*p;

	p = line;
	new_word = xcalloc(1, sizeof(char));
	while (*p)
	{
		if (is_variable(p))
			expand_variable_str(&new_word, &p, p, ctx);
		else if (is_special_parameter(p))
			expand_special_parameter_str(&new_word, &p, p, ctx);
		else
			append_char(&new_word, *p++);
	}
	free(line);
	line = NULL;
	return (new_word);
}
