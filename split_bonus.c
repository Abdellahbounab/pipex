/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 22:39:56 by abounab           #+#    #+#             */
/*   Updated: 2024/04/01 22:40:10 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static int	get_words(char const *s, char c, int *index)
{
	int	counter;
	int	found;

	counter = 0;
	if (!s)
		return (0);
	while (*s)
	{
		found = 0;
		while (*s == c && *s)
			s++;
		while (*s != c && *s)
		{
			s++;
			found = 1;
		}
		counter += found;
	}
	*index = counter;
	return (counter + 1);
}

static int	ft_strsdup(char **array, const char *s, int len)
{
	int		i;
	char	*arr;

	i = -1;
	arr = (char *)malloc(sizeof(char) * (len + 1));
	if (!arr)
		return (0);
	while (++i < len && s[i])
		arr[i] = s[i];
	arr[i] = 0;
	*array = arr;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**array;
	int		i;
	int		len;
	int		words;

	i = -1;
	words = 0;
	array = (char **)malloc(sizeof(char *) * get_words(s, c, &words));
	if (!s && !array)
		return (NULL);
	while (++i < words)
	{
		len = 0;
		while (*s == c && *s)
			s++;
		while (*s != c && *s)
		{
			len++;
			s++;
		}
		if (!ft_strsdup(&array[i], s - len, len))
			return (free_split(&array, i));
	}
	array[words] = 0;
	return (array);
}

char	**ft_special_split(char *s, char c, char condition)
{
	char	**array;
	int		i;
	int		len;
	int		words;

	i = -1;
	words = 0;
	array = (char **)malloc(sizeof(char *) * get_words(s, c, &words));
	if (!array)
		return (NULL);
	while (++i < words)
	{
		len = 0;
		while (*s && *s == c && *(s - 1) && *(s - 1) != condition)
			s++;
		while (*s && s++ && ++len)
			if (*s == c && *(s - 1) != condition)
				break ;
		if (!ft_strsdup(&array[i], s - len, len))
			return (free_split(&array, i));
	}
	return (array[words] = 0, array);
}
