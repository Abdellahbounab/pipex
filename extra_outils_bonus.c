/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra_outils_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 22:36:43 by abounab           #+#    #+#             */
/*   Updated: 2024/04/01 22:37:10 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	ft_strlcpy(char *dst, const char *src, int size)
{
	int	i;
	int	len_src;

	len_src = ft_strlen((char *)src);
	i = 0;
	if (size > i)
	{
		while (i < size - 1 && src[i])
		{
			dst[i] = (char)src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (len_src);
}
