/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arr_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 22:35:06 by abounab           #+#    #+#             */
/*   Updated: 2024/04/01 22:35:39 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	get_arr_len(char **cmd)
{
	int	i;

	i = 0;
	while (cmd && cmd[i])
		i++;
	return (i);
}

char	**arr_strdup(char **str)
{
	char	**re;
	int		i;

	if (!str)
		return (str);
	re = malloc(sizeof(char *) * (get_arr_len(str) + 1));
	if (!re)
		return (re);
	i = 0;
	while (str[i])
	{
		re[i] = ft_strdup(str[i]);
		i++;
	}
	re[i] = 0;
	return (re);
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
