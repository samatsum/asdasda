/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 12:19:12 by samatsum          #+#    #+#             */
/*   Updated: 2025/04/01 21:13:43 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include "minishell.h"

void		setup_signal(void);
void		reset_signal(void);
static int	check_state(void);

/* ************************************************************************** */
void	setup_signal(void)
{
	extern int	_rl_echo_control_chars;

	_rl_echo_control_chars = 1;
	rl_outstream = stderr;
	if (isatty(STDIN_FILENO))
		rl_event_hook = check_state;
	ignore_sig(SIGQUIT);
	handle_sig(SIGINT);
}

/* ************************************************************************** */
void	reset_signal(void)
{
	reset_sig(SIGQUIT);
	reset_sig(SIGINT);
	reset_sig(SIGPIPE);
}

/* ************************************************************************** */
static int	check_state(void)
{
	if (g_signal_received == SIGINT)
	{
		rl_replace_line("", 0);
		rl_done = 1;
		return (0);
	}
	return (0);
}
