/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 20:28:52 by abounab           #+#    #+#             */
/*   Updated: 2024/03/29 20:29:38 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**handle_args(char **arr)
{
	int		i;
	int		j;
	int		len;
	char	**head;
	char	**arr_return;

	i = 0;
	j = 1;
	head = ft_split(arr[i--], ' ');
	len = get_arr_len(head);
	if (len > 1)
	{
		len += get_arr_len(arr + 1);
		arr_return = (char **)malloc(sizeof(char *) * (len + 1));
		while (head[++i])
			arr_return[i] = ft_strdup(head[i]);
		free_arr(&head);
		while (i < len)
			arr_return[i++] = ft_strdup(arr[j++]);
		return (arr_return[i] = 0, arr_return);
	}
	return (free_arr(&head), arr_strdup(arr));
}

static int	is_setted(char c, const char *set)
{
	while (*set)
		if (c == *set)
			return (1);
	else
		set++;
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*trim;
	size_t	end;
	size_t	start;
	size_t	i;

	i = 0;
	start = 0;
	end = ft_strlen((char *)s1);
	if (!*s1)
		return (ft_strdup(""));
	while (is_setted(s1[start], set) && s1[start])
		start++;
	while (start != end && is_setted(s1[--end], set))
		;
	trim = (char *)malloc(sizeof(char) * (end - start + 1) + 1);
	if (!trim)
		return (0);
	while (s1[start] && start <= end)
	{
		trim[i] = s1[start];
		start++;
		i++;
	}
	trim[i] = 0;
	return (trim);
}
