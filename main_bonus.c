/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 21:20:15 by abounab           #+#    #+#             */
/*   Updated: 2024/04/02 18:14:49 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static char	*get_path(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return (ft_errno("Env is NULL", 1), NULL);
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5))
			return (ft_strdup(env[i] + 5));
		i++;
	}
	return (NULL);
}

void	get_input(int fd, char *delimiter)
{
	char	*ligne;

	write (0, "> ", 2);
	ligne = get_next_line(0);
	while (ligne && !ft_strncmp(ligne, delimiter, ft_strlen(ligne) - 1))
	{
		write (0, "> ", 2);
		write (fd, ligne, ft_strlen(ligne));
		ligne = get_next_line(0); 
	}
	close(fd);
}

static int	correct_commandes(char **argv, t_data **head, char *path, int *fd)
{
	int		i;
	char	**arr;
	char	**arr_tmp;
	t_data	*node;

	i = 0;
	if (!path)
		return (ft_errno("path invalid", 1), 0);
	if (*(fd + 2) > 1)
		get_input(*(fd + 2) - 1, argv[i++]);
	while (argv[i] && argv[i + 1])
	{
		arr_tmp = ft_special_split(argv[i], '\'', '\\');
		trim_array(&arr_tmp);
		arr = handle_args(arr_tmp);
		free_arr(&arr_tmp);
		if (!arr)
			return (free(path), ft_errno("malloc", 1), 0);
		node = get_cmd(arr, ft_split(path, ':'), fd);
		if (!node)
			return (free(path), free_list(head), 0);
		add_back_list(head, node);
		i++;
	}
	return (free(path), 1);
}

static int	correct_files(char *file_in, char *file_out
		, int *fd_in, int *fd_out)
{
	int	fds[2];
	int	here_doc;

	here_doc = 0;
	if (ft_strncmp(file_in, "here_doc", ft_strlen(file_in)) && pipe(fds) != -1)
	{
		here_doc = fds[1];
		*fd_in = fds[0];
		*fd_out = open(file_out, O_CREAT | O_WRONLY | O_APPEND,
				S_IRUSR | S_IWUSR | S_IRGRP);
	}
	else
	{
		*fd_in = open(file_in, O_RDONLY);
		*fd_out = open(file_out, O_CREAT | O_WRONLY | O_TRUNC,
				S_IRUSR | S_IWUSR | S_IRGRP);
	}
	if (*fd_in != -1)
	{
		if (*fd_out != -1)
			return (1 + here_doc);
		else
			return (close(*fd_in), 0);
	}
	return (0);
}

int	main(int ac, char **av, char **env)
{
	t_data	*head_cmd;
	int		fd[3];

	head_cmd = NULL;
	if (ac >= 5)
	{
		fd[2] = correct_files(av[1], av[ac - 1], &fd[0], &fd[1]);
		if ((fd[2] > 1 && ac > 5) || fd[2] == 1)
		{
			if (correct_commandes(av + 2, &head_cmd, get_path(env), fd))
				return (processing_cmds(&head_cmd, env));
			else
				ft_errno("command failed", 127);
		}
		else
			ft_errno("incorrect files or few args in here_doc", 1);
	}
	else
		ft_errno("few arguments or more", 1);
}
