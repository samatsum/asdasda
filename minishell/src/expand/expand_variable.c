/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 23:21:21 by samatsum          #+#    #+#             */
/*   Updated: 2025/03/29 17:29:59 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "xlib.h"
#include "minishell.h"

void	expand_variable_str(char **dst, char **rest, char *p, t_context *ctx);
bool	is_variable(char *s);

/* ************************************************************************** */
void	expand_variable_str(char **dst, char **rest, char *p, t_context *ctx)
{
	char	*variable_name;
	char	*value;

	variable_name = xcalloc(1, sizeof(char));
	if (*p != '$')
		assert_error("Expected dollar sign");
	p++;
	if (!is_alpha_under(*p))
		assert_error("Variable must starts with alphabets or underscore.");
	append_char(&variable_name, *p++);
	while (is_alpha_num_under(*p))
		append_char(&variable_name, *p++);
	value = xgetenv(variable_name, ctx);
	free(variable_name);
	variable_name = NULL;
	if (value)
		while (*value)
			append_char(dst, *value++);
	else
	{
		free(*dst);
		*dst = NULL;
	}
	*rest = p;
}

/* ************************************************************************** */
bool	is_variable(char *s)
{
	return (s[0] == '$' && is_alpha_under(s[1]));
}
