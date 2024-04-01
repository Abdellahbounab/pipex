/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 21:21:00 by abounab           #+#    #+#             */
/*   Updated: 2024/04/01 22:50:06 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <fcntl.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_data
{
	char			*cmd_path;
	char			**arr_cmd;
	char			*delimiter;
	char			*cmd;
	int				here_doc;
	int				parent;
	int				fd_out;
	int				fd_in;
	struct s_data	*next;
}					t_data;

int					processing_cmds(t_data **head_cmd, char **env);

char				**handle_args(char **arr);

char				**arr_strdup(char **str);

void				trim_array(char ***arr);

char				*ft_strtrim(char const *s1, char const *set);

char				**ft_special_split(char *s, char c, char condition);

char				*ft_strdup_len(char *str, int len);

t_data				*get_cmd(char **arr, char **path_arr, int *fd);

void				add_back_list(t_data **lst, t_data *newlst);

int					get_arr_len(char **cmd);

char				**ft_split(char const *s, char c);

char				**free_split(char ***array, int index);

char				*ft_strdup(char *s);

int					ft_strncmp(char *str, char *cmp, int len);

int					free_list(t_data **head);

void				ft_errno(char *str, int error);

char				*ft_strjoin(char *s1, char *s2);

char				**free_arr(char ***array);

int					ft_strlen(char *s);

int					ft_strlcpy(char *dst, const char *src, int size);

char				*get_next_line(int fd);

#endif