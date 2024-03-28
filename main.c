/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 23:08:50 by abounab           #+#    #+#             */
/*   Updated: 2024/03/28 22:42:14 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//github example : https://github.com/gabcollet/pipex/tree/master
//github documentation : https://csnotes.medium.com/pipex-tutorial-42-project-4469f5dd5901
//read about env && access && execv

void	read_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		printf("%p <<>>arr[%d] = %s<<\n", arr[i], i, arr[i]);
		i++;
	}
}

void	read_lst(t_data *lst)
{
	while (lst)
	{
		printf("(%s, %s, (%d, %d), [%d] ---> ", lst->cmd_path, lst->cmd,
			lst->fd_in, lst->fd_out, lst->parent);
		read_arr(lst->arr_cmd);
		lst = lst->next;
	}
}

static int	get_words(char const *s, char c, int *index) //ft_split
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

static int	ft_strsdup(char **array, const char *s, int len) //ft_split
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
	if (!s || !array)
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

char	*check_path(char *str, char **paths, int *len)
{
	char	*cpy;
	char	*updated;

	*len = 0;
	if (str && paths)
	{
		if (access(str, X_OK) != -1)
			return (*len = -1, ft_strdup(str));
		updated = ft_strjoin("/", str);
		while (paths[*len])
		{
			cpy = ft_strjoin(paths[*len], updated);
			if (access(cpy, X_OK) != -1)
				return (free(cpy), updated);
			free(cpy);
			cpy = NULL;
			(*len)++;
		}
		return (0);
	}
	return (0);
}

char	*get_flags(char **arr)
{
	int		i;
	char	*str;
	char	*cpy;

	i = 1;
	str = NULL;
	if (arr[i])
		str = ft_strdup(arr[i++]);
	while (str && arr && arr[i])
	{
		cpy = str;
		str = ft_strjoin(str, " ");
		free(cpy);
		cpy = str;
		str = ft_strjoin(str, arr[i]);
		free(cpy);
		i++;
	}
	return (str);
}


char	*get_path(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5))
			return (ft_strdup(env[i] + 5));
		i++;
	}
	return (NULL);
}


int	has_special(char *str, int *start, int *end, char c)
{
	while (*start < *end)
	{
		if (str[*start] != c)
			(*start)++;
		if (str[*end] != c)
			(*end)--;
		if (str[*start] == c && str[*end] == c)
			return (1);
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

int	count_char(char *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == c)
			count++;
		i++;
	}
	return (count);
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
		while (*s && s++)
		{
			len++;
			if (*s == c && *(s - 1) != condition)
				break ;
		}
		if (!ft_strsdup(&array[i], s - len, len))
			return (free_split(&array, i));
	}
	return (array[words] = 0, array);
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

int	correct_commandes(char **argv, t_data **head, char **env, int *fd) 
{
	int		i;
	char	**arr;
	char	**arr_tmp;
	t_data	*node;
	char	*path;

	i = 2;
	path = get_path(env);
	if (!path)
		return (ft_errno("path invalid", 1), 0);
	while (argv[i] && argv[i + 1])
	{
		arr_tmp = ft_special_split(argv[i], '\'', '\\');
		trim_array(&arr_tmp);
		arr = handle_args(arr_tmp);
		free_arr(&arr_tmp);
		if (!arr)
			return (ft_errno("malloc", 1), 0);
		node = get_cmd(arr, path, *fd, *(fd + 1));
		if (!node)
			return (free(path), free_list(head), 0);
		add_back_list(head, node);
		i++;
	}
	return (free(path), 1);
}

int	correct_files(char *file_in, char *file_out, int *fd_in, int *fd_out)
{
	*fd_in = open(file_in, O_RDONLY);
	*fd_out = open(file_out, O_CREAT | O_WRONLY | O_TRUNC,
			S_IRUSR | S_IWUSR | S_IRGRP);
	if (*fd_in != -1)
	{
		if (*fd_out != -1)
			return (1);
		else
			return (close(*fd_in), 0);
	}
	return (0);
}

void	last_process(t_data **head_cmd)
{
	t_data	*cpy;

	cpy = *head_cmd;
	if (!cpy)
		return ;
	while (cpy->next)
	{
		close(cpy->fd_in);
		close(cpy->fd_out);
		if (cpy->parent)
			waitpid(cpy->parent, NULL, 0);
		free_arr(&cpy->arr_cmd);
		cpy = cpy->next;
	}
}

void	processing(t_data **cpy, char **env, int fdo)
{
	dup2((*cpy)->fd_in, 0);
	if ((*cpy)->next)
		dup2(fdo, 1);
	else
	{
		close(fdo);
		dup2((*cpy)->fd_out, 1);
	}
	if ((*cpy)->cmd && (*cpy)->arr_cmd && execve((*cpy)->cmd, (*cpy)->arr_cmd,
			env) == -1)
		ft_errno("execve failed", 127);
}

void	processing_cmds(t_data **head_cmd, char **env)
{
	t_data	*cpy;
	int		pid;
	int		fds[2];

	cpy = *head_cmd;
	while (cpy)
	{
		if (pipe(fds) == -1)
			ft_errno("pid -1", -1); //update this error to free the leaks
		pid = fork();
		if (pid != -1 && !pid)
			processing(&cpy, env, fds[1]);
		else if (pid != -1 && pid)
		{
			cpy->parent = pid;
			if (cpy->next)
				dup2(fds[0], cpy->next->fd_in);
			else
				close(fds[0]);
		}
		else if (pid == -1)
		{
			last_process(head_cmd);
			free_list(head_cmd);
			close(fds[0]);
			ft_errno("pid -1", -1);
		}
		close(fds[1]);
		cpy = cpy->next;
	}
	last_process(head_cmd);
}

int	main(int ac, char **av, char **env)
{
	t_data	*head_cmd;
	int		fd[2];

	head_cmd = NULL;
	if (ac == 5)
	{
		if (correct_files(av[1], av[ac - 1], &fd[0], &fd[1]))
		{
			if (correct_commandes(av, &head_cmd, env, fd))
			{
				processing_cmds(&head_cmd, env);
				write(1, "\033[32mSuccess\033[0m\n", 17);
			}
			else
				ft_errno("command failed", 1);
		}
		else
			ft_errno("incorrect files", 1);
	}
	else
		ft_errno("few arguments or more", 1);
}
