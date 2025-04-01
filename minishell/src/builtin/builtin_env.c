/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 01:22:01 by samatsum          #+#    #+#             */
/*   Updated: 2025/04/01 17:13:05 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

int	builtin_env(char **argv, t_context *ctx);

/* ************************************************************************** */
int	builtin_env(char **argv, t_context *ctx)
{
	t_item	*cur;

	(void)argv;
	cur = ctx->envmap->item_head.next;
	while (cur)
	{
		if (!item_exported(cur))
			;
		else if (cur->value == NULL)
			;
		else
			printf("%s=%s\n", cur->name, cur->value);
		cur = cur->next;
	}
	printf("_=/usr/bin/env\n");
	return (0);
}
