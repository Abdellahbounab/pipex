/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   outils.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 22:34:30 by abounab           #+#    #+#             */
/*   Updated: 2024/03/29 20:27:29 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s && s[i++])
		;
	return (i - 1);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joined;
	size_t	total_len;
	size_t	i;

	i = 0;
	total_len = ft_strlen((char *)s1) + ft_strlen((char *)s2);
	joined = (char *)malloc(sizeof(char) * total_len + 1);
	if (!joined)
		return (NULL);
	while (i < total_len)
	{
		if (*s1)
			*(joined + i) = *((char *)s1++);
		else
			*(joined + i) = *((char *)s2++);
		i++;
	}
	*(joined + i) = 0;
	return (joined);
}

int	ft_strncmp(char *str, char *cmp, int len)
{
	int	i;

	i = 0;
	while (str && cmp && i < len)
	{
		if (str[i] != cmp[i])
			return (0);
		i++;
	}
	i--;
	if (!(str[i] - cmp[i]))
		return (1);
	return (0);
}

char	*ft_strdup(char *s)
{
	int		i;
	int		len;
	char	*arr;

	i = -1;
	len = 0;
	if (s)
	{
		len = ft_strlen(s);
		arr = (char *)malloc(sizeof(char) * (len + 1));
		if (!arr)
			return (0);
		while (++i < len && s[i])
			arr[i] = s[i];
		arr[i] = 0;
		return (arr);
	}
	return (0);
}

char	*ft_strdup_len(char *str, int len)
{
	char	*cpy;
	int		i;

	i = 0;
	if (str && !*str)
		return (ft_strdup(""));
	cpy = (char *)malloc(sizeof(char) * (len + 1));
	if (!cpy)
		return (NULL);
	while (str && str[i] && i < len)
	{
		cpy[i] = str[i];
		i++;
	}
	cpy[i] = 0;
	return (cpy);
}
