/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 18:29:01 by abounab           #+#    #+#             */
/*   Updated: 2024/03/28 18:35:29 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

#include <unistd.h>
#include <stdio.h> //have to remembre it and the read funciton to debug
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

typedef struct s_data{
    char *cmd_path;
	char **arr_cmd;
	char *cmd;
	int parent;
	int fd_out;
	int fd_in;
	struct s_data *next;
}   t_data;

void	processing_cmds(t_data **head_cmd, char **env);

void	processing(t_data **cpy,  char **env, int fdo);

void	last_process(t_data **head_cmd);

t_data *get_list(t_data *head, int index);

int correct_files(char *file_in, char *file_out, int *fd_in, int *fd_out);

int correct_commandes(char **argv, int len, t_data **head, char **env, int *fd);

char **handle_args(char **arr);

char **arr_strdup (char **str);

void trim_array(char ***arr);

char	*ft_strtrim(char const *s1, char const *set);

char **ft_special_split(char *s, char c, char condition);

int count_char(char *str, char c);

char *ft_strdup_len(char *str, int len);

int has_special(char *str, int *start, int *end, char c);

int strlen_lst(t_data *head);

void	add_back_list(t_data **lst, t_data *newlst);

char *get_path(char **env);

t_data *get_cmd(char **arr, char *paths, int file_in, int file_out);

char **get_arr_cmd(char **cmd);

int get_arr_len(char **cmd);

char *get_flags(char **arr);

char *check_path(char *str ,char **paths, int *len);

char	**ft_split(char const *s, char c);

char	**free_split(char ***array, int index);

char	*ft_strdup(char *s);

int	ft_strncmp(char *str, char *cmp, int len);

int free_list(t_data **head);

void ft_errno(char *str);//have to mpdify it to get an int where we can update the exit function

char	*ft_strjoin(char const *s1, char const *s2);

char	**free_arr(char ***array);

int ft_strlen(char *s);


#endif