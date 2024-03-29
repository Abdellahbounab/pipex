/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 19:30:01 by abounab           #+#    #+#             */
/*   Updated: 2024/03/29 22:32:05 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include "pipex.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

char	*get_next_line(int fd);

int	until_line(char *str);

int	ft_strlcpy(char *dst, const char *src, int size);

char	*ft_get_line(char *str);

char	*ft_read_buff(int fd, char *str);

char	*ft_update(char *str);

#endif