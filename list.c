/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 22:38:54 by abounab           #+#    #+#             */
/*   Updated: 2024/03/28 22:40:44 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	strlen_lst(t_data *head)
{
	int	len;

	len = 0;
	while (head)
	{
		head = head->next;
		len++;
	}
	return (len);
}

t_data	*get_list(t_data *head, int index)
{
	int	i;

	i = 0;
	while (head && i < index)
	{
		head = head->next;
		i++;
	}
	return (head);
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

t_data	*get_cmd(char **arr, char *paths, int file_in, int file_out)
{
	int		index_path;
	char	**path_arr;
	char	*updated;
	t_data	*node;

	path_arr = ft_split(paths, ':');
	updated = check_path(arr[0], path_arr, &index_path);
	if (path_arr && updated)
	{
		node = (t_data *)malloc(sizeof(t_data));
		if (node)
		{
			if (index_path != -1)
			{
				node->cmd_path = ft_strdup(path_arr[index_path]);
				if (!node->cmd_path)
					return (free(updated), free(node), NULL);
				node->cmd = ft_strjoin(node->cmd_path, updated);
			}
			else
			{
				node->cmd_path = NULL;
				node->cmd = ft_strdup(updated);
			}
			free_arr(&path_arr);
			if (!node->cmd)
				return (free(updated), free(node->cmd_path), free(node), NULL);
			node->arr_cmd = arr;
			if (!node->arr_cmd)
				return (free(node->cmd), free(updated), free(node), NULL);
			node->parent = 0;
			node->fd_in = file_in;
			node->fd_out = file_out;
			node->next = NULL;
			return (free(updated), node);
		}
		else
			return (free(updated), free_arr(&path_arr), NULL);
	}
	else
		return (free(updated), free_arr(&path_arr),
			ft_errno("failed command", 1), NULL);
}
