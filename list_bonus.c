/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 22:37:56 by abounab           #+#    #+#             */
/*   Updated: 2024/04/02 18:11:52 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static char	*check_path(char *str, char **paths, int *len)
{
	char	*cpy;
	char	*updated;

	*len = 0;
	if (str && paths)
	{
		if (access(str, X_OK) != -1)
			return (*len = -1, ft_strdup(str));
		updated = ft_strjoin("/", str);
		if (updated)
		{
			while (paths[*len])
			{
				cpy = ft_strjoin(paths[*len], updated);
				if (access(cpy, X_OK) != -1)
					return (free(cpy), updated);
				free(cpy);
				cpy = NULL;
				(*len)++;
			}
		}
		return (0);
	}
	return (0);
}

void	add_back_list(t_data **lst, t_data *newlst)
{
	t_data	*tmp;

	if (!*lst)
		*lst = newlst;
	else
	{
		tmp = *lst;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = newlst;
	}
}

static t_data	*create_node(int file_in, int file_out, char **arr)
{
	t_data	*node;

	node = (t_data *)malloc(sizeof(t_data));
	if (!node)
		return (NULL);
	node->parent = 0;
	node->fd_in = file_in;
	node->fd_out = file_out;
	node->next = NULL;
	node->cmd = NULL;
	node->arr_cmd = arr;
	return (node);
}

t_data	*get_cmd(char **arr, char **path_arr, int *fd)
{
	int		i;
	char	*updated;
	t_data	*node;

	updated = check_path(arr[0], path_arr, &i);
	if (path_arr && updated)
	{
		node = create_node(*fd, *(fd + 1), arr);
		if (node)
		{
			if (i != -1)
				node->cmd = ft_strjoin(path_arr[i], updated);
			else
				node->cmd = ft_strdup(updated);
			free_arr(&path_arr);
			if (!node->cmd)
				return (free(updated), free(node), NULL);
			return (free(updated), node);
		}
		else
			return (free(updated), free_arr(&path_arr), NULL);
	}
	else
		return (free(updated), free_arr(&path_arr),
			ft_errno("failed command", 127), NULL);
}
