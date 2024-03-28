/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arr.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 22:32:56 by abounab           #+#    #+#             */
/*   Updated: 2024/03/28 22:33:52 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	get_arr_len(char **cmd)
{
	int	i;

	i = 0;
	while (cmd && cmd[i])
		i++;
	return (i);
}

void	trim_array(char ***arr)
{
	int		i;
	int		j;
	char	**arr_cpy;
	char	*tmp;

	i = 0;
	j = 0;
	arr_cpy = *arr;
	while (arr_cpy[i])
	{
		tmp = arr_cpy[i];
		arr_cpy[j] = ft_strtrim(tmp, " ");
		if (!*arr_cpy[j])
		{
			free(arr_cpy[j]);
			arr_cpy[j] = NULL;
			j--;
		}
		free(tmp);
		tmp = NULL;
		j++;
		i++;
	}
	while (j < i)
		arr_cpy[j++] = 0;
}
