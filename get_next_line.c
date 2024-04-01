/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 18:58:59 by abounab           #+#    #+#             */
/*   Updated: 2024/03/30 16:11:33 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	until_line(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	return (i);
}

char	*ft_read_buff(int fd, char *str)
{
	char	buff[BUFFER_SIZE + 1];
	int		count;
	char	*tmp;

	count = 1;
	while (count)
	{
		tmp = str;
		count = read(fd, buff, BUFFER_SIZE);
		if (count < 0)
			return (free(str), NULL);
		buff[count] = 0;
		str = ft_strjoin(tmp, buff);
		free(tmp);
		if (!str)
			return (0);
		if (until_line(buff) < ft_strlen(buff))
			break ;
		buff[0] = 0;
	}
	return (str);
}

char	*ft_get_line(char *str)
{
	int		len;
	char	*cpy;

	if (!*str)
		return (0);
	len = until_line(str) + 1;
	if (str[until_line(str)])
		len++;
	cpy = (char *) malloc (sizeof(char) * len);
	if (!cpy)
		return (0);
	return (ft_strlcpy(cpy, str, len), cpy);
}

char	*ft_update(char *str)
{
	char	*cpy;

	if (!str[until_line(str)])
		return (0);
	cpy = ft_strdup(str + until_line(str) + 1);
	if (!cpy)
		return (0);
	return (cpy);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*str;
	char		*tmp;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
		return (free(str), str = NULL, NULL);
	tmp = ft_read_buff(fd, str);
	if (!tmp)
		return (str = NULL, NULL);
	line = ft_get_line(tmp);
	str = ft_update(tmp);
	free(tmp);
	tmp = NULL;
	if (!line)
		return (free(str), str = NULL, NULL);
	return (line);
}
