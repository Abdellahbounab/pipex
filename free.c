/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 22:41:20 by abounab           #+#    #+#             */
/*   Updated: 2024/03/28 22:42:20 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
//have to mpdify it to get an int where we can update the exit function
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
