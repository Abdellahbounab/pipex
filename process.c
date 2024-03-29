/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 20:38:28 by abounab           #+#    #+#             */
/*   Updated: 2024/03/29 23:07:35 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	last_process(t_data **head_cmd)
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

static void	processing(t_data **cpy, char **env, int fdo)
{
	dup2((*cpy)->fd_in, 0);
	// if (!(*cpy)->fd_in)
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

static void	processing_cmds_outils(t_data **node, int pid, int fd_zero)
{
	if (node)
	{
		(*node)->parent = pid;
		if ((*node)->next)
			dup2(fd_zero, (*node)->next->fd_in);
		else
			close(fd_zero);
	}
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
			return (last_process(head_cmd), 
				free_list(head_cmd), ft_errno("pid -1", -1));
		pid = fork();
		if (pid != -1 && !pid)
			processing(&cpy, env, fds[1]);
		else if (pid != -1 && pid)
			processing_cmds_outils(&cpy, pid, fds[0]);
		else
			return (last_process(head_cmd), free_list(head_cmd),
				close(fds[0]), ft_errno("pid -1", -1));
		close(fds[1]);
		cpy = cpy->next;
	}
	last_process(head_cmd);
}
