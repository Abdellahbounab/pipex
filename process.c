/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 20:38:28 by abounab           #+#    #+#             */
/*   Updated: 2024/04/02 18:01:01 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	last_process(t_data **head_cmd)
{
	t_data	*cpy;
	int		status;

	cpy = *head_cmd;
	if (!cpy)
		return (1);
	while (cpy)
	{
		close(cpy->fd_in);
		close(cpy->fd_out);
		if (cpy->parent)
		{
			if (waitpid(cpy->parent, &status, 0) == -1)
				return (WEXITSTATUS(status));
		}
		free_arr(&cpy->arr_cmd);
		cpy = cpy->next;
	}
	return (WEXITSTATUS(status));
}

static void	processing(t_data **cpy, char **env, int fdo, int fdi)
{
	close(fdi);
	dup2((*cpy)->fd_in, 0);
	if ((*cpy)->next)
	{
		dup2(fdo, 1);
		close(fdo);
	}
	else
	{
		close(fdo);
		dup2((*cpy)->fd_out, 1);
	}
	if ((*cpy)->cmd && (*cpy)->arr_cmd && execve((*cpy)->cmd, (*cpy)->arr_cmd,
			env) == -1)
		ft_errno("execve failed", 127);
}

static void	processing_cmds_outils(t_data **node, int pid, int fd_zero, int fdo)
{
	if (node)
	{
		close(fdo);
		(*node)->parent = pid;
		if ((*node)->next)
			dup2(fd_zero, (*node)->next->fd_in);
		close(fd_zero);
	}
}

int	processing_cmds(t_data **head_cmd, char **env)
{
	t_data	*cpy;
	int		pid;
	int		fds[2];

	cpy = *head_cmd;
	while (cpy)
	{
		if (pipe(fds) == -1)
			return (free_list(head_cmd), last_process(head_cmd));
		pid = fork();
		if (pid != -1 && !pid)
			processing(&cpy, env, fds[1], fds[0]);
		else if (pid != -1 && pid)
			processing_cmds_outils(&cpy, pid, fds[0], fds[1]);
		else
			return (free_list(head_cmd), close(fds[0]),
				last_process(head_cmd));
		cpy = cpy->next;
	}
	return (last_process(head_cmd));
}
