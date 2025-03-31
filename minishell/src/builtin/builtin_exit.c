/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 23:02:03 by samatsum          #+#    #+#             */
/*   Updated: 2025/03/31 20:42:44 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <errno.h>
#include "minishell.h"

int	builtin_exit(char **argv, t_context *ctx, t_node *node);

int	builtin_exit(char **argv, t_context *ctx, t_node *node)
{
	long	res;
	char	*endptr;

	if (node->process_pid != 0)
		write(1, "exit\n", 6);
	//free_node(node);
	if (argv[1] == NULL)
		exit(ctx->last_status);
	errno = 0;
	res = ft_strtol(argv[1], &endptr, 10);
	if (errno != 0 || *endptr != '\0')
	{
		xperror3("exit", argv[1], "numeric argument required");
		exit(2);
	}
	if (argv[2])
	{
		xperror2("exit", "too many arguments");
		return (1);
	}
	exit((unsigned char)res);
}
