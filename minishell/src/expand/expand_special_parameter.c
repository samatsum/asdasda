/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_special_parameter.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 13:34:26 by samatsum          #+#    #+#             */
/*   Updated: 2025/03/29 17:29:51 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		expand_special_parameter_str(char **dst, char **rest, char *p, \
	t_context *ctx);
static void	append_num(char **dst, unsigned int num);
bool		is_special_parameter(char *s);

/* ************************************************************************** */
void	expand_special_parameter_str(char **dst, char **rest, char *p, \
	t_context *ctx)
{
	if (!is_special_parameter(p))
		assert_error("Expected special parameter");
	p += 2;
	append_num(dst, ctx->last_status);
	*rest = p;
}

/* ************************************************************************** */
static void	append_num(char **dst, unsigned int num)
{
	if (num == 0)
	{
		append_char(dst, '0');
		return ;
	}
	if (num / 10 != 0)
		append_num(dst, num / 10);
	append_char(dst, '0' + (num % 10));
}

/* ************************************************************************** */
bool	is_special_parameter(char *s)
{
	return (s[0] == '$' && s[1] == '?');
}
