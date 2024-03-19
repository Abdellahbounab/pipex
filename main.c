#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>


//github example : https://github.com/gabcollet/pipex/tree/master
//github documentation : https://csnotes.medium.com/pipex-tutorial-42-project-4469f5dd5901
//read about env && access && execv


typedef struct s_data{
    char *cmd_path;
    char *cmd_flags;
	char *cmd;
	int parent;//t_pid
	struct s_data *next;
}   t_data;

int ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s && s[i++])
		;
	return (i - 1);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joined;
	size_t	total_len;
	size_t	i;

	i = 0;
	total_len = ft_strlen((char *)s1) + ft_strlen((char *)s2);
	joined = (char *) malloc(sizeof(char) * total_len + 1);
	if (!joined)
		return (NULL);
	while (i < total_len)
	{
		if (*s1)
			*(joined + i) = *((char *)s1++);
		else
			*(joined + i) = *((char *)s2++);
		i++;
	}
	*(joined + i) = 0;
	return (joined);
}

void ft_errno()
{
	write (2, "Error\n", 6);
	exit (EXIT_FAILURE);
}

int free_list(t_data **head)
{
	t_data *tmp;

	while (*head)
	{
		tmp = (*head)->next;
		free(*head);
		*head = tmp;
	}
	return (1);
}

int	ft_strncmp(char *str, char *cmp, int len)
{
	int	i;

	i = 0;
	while (str && cmp && i < len)
	{
		if (str[i] != cmp[i])
			return (0);
		i++;
	}
	i--;
	if (!(str[i] - cmp[i]))
		return (1);
	return (0);
}

static int	get_words(char const *s, char c, int *index) //ft_split
{
	int		counter;
	int		found;

	counter = 0;
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
	arr = (char *) malloc (sizeof(char) * (len + 1));
	if (!arr)
		return (0);
	while (++i < len && s[i])
		arr[i] = s[i];
	arr[i] = 0;
	*array = arr;
	return (1);
}

char	*ft_strdup(char *s)
{
	int		i;
	int		len;
	char	*arr;

	i = -1;
	len = 0;
	if (s)
	{
		len = ft_strlen(s);
		arr = (char *) malloc (sizeof(char) * (len + 1));
		if (!arr)
			return (0);
		while (++i < len && s[i])
			arr[i] = s[i];
		arr[i] = 0;
		return (arr);
	}
	return (0);
}

char	**free_split(char ***array, int index)
{
	int	i;

	i = 0;
	while (i < index)
		free((*array)[i++]);
	free(*array);
	return (NULL);
}

char	**free_arr(char ***array)
{
	int	i;

	i = 0;
	while (array && *array && (*array)[i])
	{
		free((*array)[i]);
		i++;
	}
	free(*array);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**array;
	int		i;
	int		len;
	int		words;

	i = -1;
	words = 0;
	array = (char **) malloc (sizeof(char *) * get_words(s, c, &words));
	if (!array)
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

int check_path(char *str ,char **paths)
{
	int i;
	char *cpy;

	i = 0;
	if (str && paths)
	{
		while (paths[i])
		{
			cpy = ft_strjoin(paths[i], str);
			if (access(cpy, X_OK) != -1)
				return (free(cpy), i);
			free(cpy);
			cpy = NULL;
			i++;
		}
		return (-1);
	}
	return (-1);
}

char *get_flags(char **arr)
{
	int i = 1;
	char *str;
	char *cpy;

	if (arr[i])
		str = ft_strdup(arr[i++]);
	while (str && arr && arr[i])
	{
		cpy = str;
		str = ft_strjoin(str, arr[i]);
		free(cpy);
		i++;
	}
	return (str);
}

t_data *get_cmd(char **arr, char *paths)
{
	int index_path;
	char **path_arr;
	char *updated;
	t_data *node;

	path_arr = ft_split(paths, ':');
	updated = ft_strjoin("/", arr[0]);
	index_path = check_path(updated, path_arr);
	if (path_arr && index_path != -1)
	{
		node = (t_data *) malloc (sizeof(t_data));
		if (node)
		{
			node->cmd_path = ft_strdup(path_arr[index_path]);
			free_arr(&path_arr);//seg buffer-overflow
			if (!node->cmd_path)
				return (free(updated), free(node), NULL);
			node->cmd = ft_strjoin(node->cmd_path, updated);
			if (!node->cmd)
				return (free(updated), free(node->cmd_path), free(node), NULL);
			node->cmd_flags = get_flags(arr);
			node->next = NULL;
			return (free(updated), node);
		}
		else
			return (free(updated), free_arr(&path_arr), NULL);
	}
	else
		return (free(updated), free_arr(&path_arr), ft_errno(), NULL);
}

char *get_path(char **env)
{
	int i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5))
			return (ft_strdup(env[i] + 5));
		i++;
	}
	return (NULL);
}

void	add_back_list(t_data **lst, t_data *new)
{
	t_data	*tmp;

	if (!*lst)
		*lst = new;
	else
	{
		tmp = *lst;
		while (tmp -> next)
			tmp = tmp -> next;
		tmp -> next = new;
	}
}

int correct_commandes(char **argv, int len, t_data **head, char **env)
{
    int i;
	char **arr;
	t_data *node;
	char *path;

    i = 2;
	path = get_path(env);	//getting path through the path got by get_path
	if (!path)
		return (ft_errno(), 0);
    while (i < len)
    {
        arr = ft_split(argv[i], ' ');
        node = get_cmd(arr, path);
		free_arr(&arr);
		if (!node)
			return (free_list(head), 0);	//have to free all linked list (head & arr) & return ft_errno
		add_back_list(head, node);
        i++;
    }
	return (1);
}

int correct_files(char *file_in, char *file_out)
{
    int fd_in;
    int fd_out;

    fd_in = open(file_in, O_RDONLY);
    fd_out = open(file_out, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP);
    if (fd_in != -1)
	{
		dup2(STDIN_FILENO, fd_in);
        if (fd_out != -1)
		{
			dup2(STDOUT_FILENO, fd_out);
            return (1);
		}
		else
			return (close(fd_in), 0);
	}
    return (0);
}

void pipe_all(int ***fd, int len)
{
	int i;

	i = 0;
	while (i < len)
	{
		if (pipe((*fd)[i]) == -1)
			ft_errno();
		i++;
	}
}

void	close_fd(int ***fd, int len)
{
	int i;

	i = 0;
	while (i < len)
	{
		close(*fd[i][0]);
		close(*fd[i][1]);
		i++;
	}
}

void	processing(int *fd, t_data **head_cmd, t_data *head)
{
	if (head != *head_cmd || (*head_cmd)->next)
	{

	}
	else
	{
		close(fd[1]);
		dup2(0, fd[0]);
		dup2(1, fd[1]);
	}
	// input(read) from fd[1] tha is 0 now
	//	write to output
}

// have to update the wait of pid and make the parent last thing and only child who excve cmds , make all pids as an array to make the wait easier and logical
void	processing_cmds(t_data **head_cmd)
{
	int fd[strlen_lst(*head_cmd) - 1][2];
	t_data *cpy;
	int i;
	int pid;

	cpy = *head_cmd;
	i = 0;
	pipe_all(&fd, strlen_lst(*head_cmd) - 1);
	while (cpy->next)
	{
		pid = fork();
		if (pid != -1 && !pid)
			processing(fd[i], cpy, *head_cmd);
		else if (pid != -1 && pid)
			cpy->parent = pid;
		else if (pid == -1)
		{
			close_fd(&fd, ft_strlen(*head_cmd) - 1);
			free_list(head_cmd);
			ft_errno();
		}
		cpy = cpy->next;
		i++;
	}
	last_process(head_cmd);
}


void read_lst(t_data *lst)
{
	while (lst)
	{
		printf("(%s, %s, %s)\n", lst->cmd_path, lst->cmd, lst->cmd_flags);
		lst = lst -> next;
	}
}

int main(int ac, char **av, char **env)
{
    t_data *head_cmd;

	head_cmd = NULL;
    if (ac >= 5)
	{
        if (correct_files(av[1], av[ac - 1]))
		{
            if (correct_commandes(av, ac - 1, &head_cmd, env))
					processing_cmds(&head_cmd);
		}
		else
			// ft_errno();
			printf("error files");
	}
	else
		// ft_errno();
		printf("error arguments < 5");
}
