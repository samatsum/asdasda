/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_map_put.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:32:54 by samatsum          #+#    #+#             */
/*   Updated: 2025/04/01 21:14:14 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "xlib.h"
#include "minishell.h"

/* ************************************************************************** */
int			map_put(t_map *envmap, const char *string, \
						bool allow_empty, int attr);
static void	set_name_value(t_map *envmap, const char *string, \
	char **name, char **value);
static void	handle_append_value(t_map *envmap, char *name, \
	char **value, char *append_str);

/* ************************************************************************** */
int	map_put(t_map *envmap, const char *string, bool allow_empty, int attr)
{
	int		result;
	char	*name;
	char	*value;

	set_name_value(envmap, string, &name, &value);
	if (value == NULL && !allow_empty)
	{
		free(name);
		name = NULL;
		return (-1);
	}
	result = map_set_attr(envmap, name, value, attr);
	free(name);
	name = NULL;
	free(value);
	value = NULL;
	return (result);
}

/* ************************************************************************** */
static void	set_name_value(t_map *envmap, const char *string,
	char **name, char **value)
{
	char	*name_end;
	char	*name_end_append;

	name_end = ft_strchr(string, '=');
	name_end_append = ft_strnstr(string, "+=", ft_strlen(string));
	if (name_end == NULL)
	{
		*name = xstrdup(string);
		*value = NULL;
	}
	else if (name_end && name_end_append == NULL)
	{
		*name = xstrndup(string, name_end - string);
		*value = xstrdup(name_end + 1);
	}
	else
	{
		*name = xstrndup(string, name_end_append - string);
		handle_append_value(envmap, *name, value, name_end_append + 2);
	}
}

/* ************************************************************************** */
static void	handle_append_value(t_map *envmap, char *name,
	char **value, char *append_str)
{
	char	*original_value;
	char	*tmp_value;

	original_value = xgetenv(name, envmap);
	tmp_value = xstrdup(append_str);
	if (tmp_value[0] == '\0' && original_value)
	{
		*value = ft_strjoin(original_value, "");
		free(tmp_value);
		tmp_value = NULL;
	}
	else if (original_value)
	{
		*value = ft_strjoin(original_value, tmp_value);
		free(tmp_value);
		tmp_value = NULL;
	}
	else
		*value = tmp_value;
}
