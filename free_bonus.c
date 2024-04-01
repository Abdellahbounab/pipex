/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 22:37:28 by abounab           #+#    #+#             */
/*   Updated: 2024/04/01 22:37:40 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	free_list(t_data **head)
{
	t_data	*tmp;

	while (*head)
	{
		tmp = (*head)->next;
		free_arr(&(*head)->arr_cmd);
		free(*head);
		*head = tmp;
	}
	return (1);
}

void	ft_errno(char *str, int error)
{
	write(2, "\033[31mError :", 12);
	write(2, str, ft_strlen(str));
	write(2, "\033[0m\n", 5);
	exit(error);
}

char	**free_arr(char ***array)
{
	int	i;

	i = 0;
	while (array && *array && (*array)[i])
	{
		free((*array)[i]);
		(*array)[i] = NULL;
		i++;
	}
	free(*array);
	return (NULL);
}

char	**free_split(char ***array, int index)
{
	int	i;

	i = 0;
	while (i < index)
		free((*array)[i++]);
	free(*array);
	return (NULL);
}
