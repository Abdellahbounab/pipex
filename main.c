/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 23:08:50 by abounab           #+#    #+#             */
/*   Updated: 2024/04/01 22:50:48 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//delete this reminder before pushing
//read about env && access && execv

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

static int	correct_commandes(char **argv, t_data **head, char *path, int *fd)
{
	int		i;
	char	**arr;
	char	**arr_tmp;
	t_data	*node;

	i = 0;
	if (!path)
		return (ft_errno("path invalid", 1), 0);
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
	*fd_in = open(file_in, O_RDONLY);
	if (*fd_in != -1)
	{
		*fd_out = open(file_out, O_CREAT | O_WRONLY | O_TRUNC,
				S_IRUSR | S_IWUSR | S_IRGRP);
		if (*fd_out != -1)
			return (1);
		else
			return (close(*fd_in), 0);
	}
	return (0);
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
			if (correct_commandes(av + 2, &head_cmd, get_path(env), fd))
				return (processing_cmds(&head_cmd, env));
			else
				ft_errno("command failed", 127);
		}
		else
			ft_errno("incorrect files", 1);
	}
	else
		ft_errno("few arguments or more", 1);
}
