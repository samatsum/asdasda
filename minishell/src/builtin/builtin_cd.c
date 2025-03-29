/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 00:24:53 by samatsum          #+#    #+#             */
/*   Updated: 2025/03/29 17:30:46 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include "xlib.h"
#include "minishell.h"

int			builtin_cd(char **argv, t_context *ctx);
static char	*resolve_pwd(char *oldpwd, char *path);
static void	update_oldpwd(char *pwd, t_context *ctx);
static int	set_path(char *path, size_t path_size, char *arg, t_context *ctx);

/* ************************************************************************** */
int	builtin_cd(char **argv, t_context *ctx)
{
	char	*pwd;
	char	path[PATH_MAX];
	char	*newpwd;

	if (argv[2])
	{
		xperror2("cd", "too many arguments");
		return (1);
	}
	pwd = xgetenv("PWD", ctx);
	update_oldpwd(pwd, ctx);
	if (set_path(path, PATH_MAX, argv[1], ctx) < 0)
		return (1);
	if (chdir(path) < 0)
	{
		xperror3("cd", path, NULL);
		return (1);
	}
	newpwd = resolve_pwd(pwd, path);
	map_set(ctx->envmap, "PWD", newpwd);
	free(newpwd);
	newpwd = NULL;
	return (0);
}

/* ************************************************************************** */
static char	*resolve_pwd(char *oldpwd, char *path)
{
	char	newpwd[PATH_MAX];
	char	*dup;

	if (*path == '/' || oldpwd == NULL)
		ft_strlcpy(newpwd, "/", PATH_MAX);
	else
		ft_strlcpy(newpwd, oldpwd, PATH_MAX);
	while (*path)
	{
		if (*path == '/')
			path++;
		else if (consume_path(&path, path, "."))
			;
		else if (consume_path(&path, path, ".."))
			delete_last_path_elm(newpwd);
		else
			append_path_elm(newpwd, PATH_MAX, &path, path);
	}
	dup = xstrdup(newpwd);
	return (dup);
}

/* ************************************************************************** */
static void	update_oldpwd(char *pwd, t_context *ctx)
{
	if (pwd == NULL)
		map_set(ctx->envmap, "OLDPWD", "");
	else
		map_set(ctx->envmap, "OLDPWD", pwd);
}

/* ************************************************************************** */
static int	set_path(char *path, size_t path_size, char *arg, t_context *ctx)
{
	char	*home;

	if (arg == NULL)
	{
		home = xgetenv("HOME", ctx);
		if (home == NULL)
		{
			xperror2("cd", "HOME not set");
			return (-1);
		}
		ft_strlcpy(path, home, path_size);
	}
	else
		ft_strlcpy(path, arg, path_size);
	return (0);
}
