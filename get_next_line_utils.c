/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 19:08:37 by abounab           #+#    #+#             */
/*   Updated: 2024/03/29 22:32:16 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"




//update the header , creating it again for both getnextline




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
