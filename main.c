#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>


//github example : https://github.com/gabcollet/pipex/tree/master
//github documentation : https://csnotes.medium.com/pipex-tutorial-42-project-4469f5dd5901
//read about env && access && execv


typedef struct s_data{
    char *cmd_path;
    char *cmd_flags;
	char **arr_cmd;
	char *cmd;
	int parent;//t_pid
	int fd_out;
	int fd_in;
	struct s_data *next;
}   t_data;

void read_arr(char **arr)
{
	int i;

	i = 0;
	while (arr[i])
	{
		printf("arr[%d] = %s\n", i, arr[i]);
		i++;
	}
}

void read_lst(t_data *lst)
{
	while (lst)
	{
		printf("(%s, %s, %s, (%d, %d), [%d] ---> ", lst->cmd_path, lst->cmd, lst->cmd_flags, lst->fd_in, lst->fd_out, lst->parent);
		read_arr(lst->arr_cmd);
		lst = lst -> next;
	}
}

int ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s && s[i++])
		;
	return (i - 1);
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

void ft_errno(char *str)
{
	write (2, "\033[31mError :", 12);
	write (2, str, ft_strlen(str));
	write (2, "\033[0m\n", 5);
	exit (EXIT_FAILURE);
}

int free_list(t_data **head)
{
	t_data *tmp;

	while (*head)
	{
		tmp = (*head)->next;
		free_arr(&(*head)->arr_cmd);
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


	str = NULL;
	// special_handler(); ==> to handle the special characters such as " ' " using the trim function
	if (arr[i])
		str = ft_strdup(arr[i++]);
	printf("str : %s\n", str);
	while (str && arr && arr[i])
	{
		cpy = str;
		str = ft_strjoin(str, arr[i]);
		free(cpy);
		i++;
	}
	return (str);
}

t_data *get_cmd(char **arr, char *paths, int file_in, int file_out)
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
			free_arr(&path_arr);
			if (!node->cmd_path)
				return (free(updated), free(node), NULL);
			node->cmd = ft_strjoin(node->cmd_path, updated);
			if (!node->cmd)
				return (free(updated), free(node->cmd_path), free(node), NULL);
			node->arr_cmd = arr; //have to be freeing after finihsing
			node->parent = 0;
			node->cmd_flags = get_flags(arr);
			node->fd_in = file_in;
			node->fd_out = file_out;
			node->next = NULL;
			return (free(updated), node);
		}
		else
			return (free(updated), free_arr(&path_arr), NULL);
	}
	else
		return (free(updated), free_arr(&path_arr), ft_errno("failed command"), NULL);
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

int strlen_lst(t_data *head)
{
	int len;

	len = 0;
	while (head)
	{
		head = head->next;
		len++;
	}
	return (len);
}

int correct_commandes(char **argv, int len, t_data **head, char **env, int *fd)
{
    int i;
	char **arr;
	t_data *node;
	char *path;

    i = 2;
	path = get_path(env);	//getting path through the path got by get_path
	if (!path)
		return (ft_errno("path invalid"), 0);
    while (i < len)
    {
        arr = ft_split(argv[i], ' ');
		if (!arr)
			return (ft_error("malloc"), 0);
		handle_args(&arr);
        node = get_cmd(arr, path, *fd, *(fd + 1));
		if (!node)
			return (free_list(head), 0);	//have to free all linked list (head & arr) & return ft_errno
		add_back_list(head, node);
        i++;
    }
	return (1);
}

int correct_files(char *file_in, char *file_out, int *fd_in, int *fd_out)
{
    *fd_in = open(file_in, O_RDONLY);
    *fd_out = open(file_out, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP);
    if (*fd_in != -1)
	{
        if (*fd_out != -1)
            return (1);
		else
			return (close(*fd_in), 0);
	}
    return (0);
}

t_data *get_list(t_data *head, int index)
{
	int i;

	i = 0;
	while (head && i < index)
	{
		head = head -> next;
		i++;
	}
	return (head);
}

void	last_process(t_data **head_cmd)
{
	t_data *cpy;

	cpy = *head_cmd;
	if (!cpy)
		return ;
	while (cpy -> next)
	{
		close(cpy->fd_in);
		close(cpy->fd_out);
		if (cpy->parent)
			waitpid(cpy->parent, NULL, 0);
		free_arr(&cpy->arr_cmd);
		cpy = cpy->next;
	}
}

void	processing(t_data **cpy,  char **env, int fdo)
{
	dup2((*cpy)->fd_in, 0);
	if ((*cpy)->next)
		dup2(fdo, 1);
	else 
	{
		close(fdo);
		dup2((*cpy)->fd_out, 1);
	}
	if ((*cpy)->cmd && (*cpy)->arr_cmd && execve((*cpy)->cmd, (*cpy)->arr_cmd, env) == -1)
		ft_errno("execve failed");
}

// have to update the wait of pid and make the parent last thing and only child who excve cmds , make all pids as an array to make the wait easier and logical
void	processing_cmds(t_data **head_cmd, char **env)
{
	t_data *cpy;
	int pid;
	int fds[2];

	cpy = *head_cmd;
	while (cpy)
	{
		if (pipe(fds) == -1)
			ft_errno("pid -1");//update this error to free the leaks
		pid = fork();
		if (pid != -1 && !pid)
		{
			processing(&cpy, env, fds[1]);
		}
		else if (pid != -1 && pid)
		{
			cpy->parent = pid;
			if (cpy->next)
			{
				close(cpy->next->fd_in);
				cpy->next->fd_in = fds[0];
			}
			else
				close(fds[0]);
		}
		else if (pid == -1)
		{
			last_process(head_cmd);
			free_list(head_cmd);
			close(fds[0]);
			ft_errno("pid -1");
		}
		close(fds[1]);
		cpy = cpy->next;
	}
	last_process(head_cmd);
}

int main(int ac, char **av, char **env)
{
    t_data *head_cmd;
	int fd[2];

	head_cmd = NULL;
    if (ac >= 5)
	{
        if (correct_files(av[1], av[ac - 1], &fd[0], &fd[1]))
		{
            if (correct_commandes(av, ac - 1, &head_cmd, env, fd))//handle the ' ' commandes and the single ones
					processing_cmds(&head_cmd, env);
			write(1, "\033[32mSuccess\033[0m\n", 17);
		}
		else
			ft_errno("incorrect files");
	}
	else
		ft_errno("few arguments");
}
