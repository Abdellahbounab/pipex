
#include "pipex.h"

//github example : https://github.com/gabcollet/pipex/tree/master
//github documentation : https://csnotes.medium.com/pipex-tutorial-42-project-4469f5dd5901
//read about env && access && execv



void read_arr(char **arr)
{
	int i;

	i = 0;
	while (arr[i])
	{
		printf("%p <<>>arr[%d] = %s<<\n",arr[i], i, arr[i]);
		i++;
	}
}

void read_lst(t_data *lst)
{
	while (lst)
	{
		printf("(%s, %s, (%d, %d), [%d] ---> ", lst->cmd_path, lst->cmd, lst->fd_in, lst->fd_out, lst->parent);
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

void ft_errno(char *str)//have to mpdify it to get an int where we can update the exit function
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

char *check_path(char *str ,char **paths, int *len)
{
	char *cpy;
	char *updated;

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

char *get_flags(char **arr)
{
	int i = 1;
	char *str;
	char *cpy;


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

int get_arr_len(char **cmd)
{
	int i;

	i = 0;
	while (cmd && cmd[i])
		i++;
	return (i);
}


char **get_arr_cmd(char **cmd)
{
	int len;
	char **arr;
	int i;

	i = 0;
	len = get_arr_len(cmd);
	// have to update this arr** to contain every flag in its own memory address:
	arr = (char **) malloc (sizeof(char *) * len + 1);
	if (!arr)
		return (NULL);
	arr[i] = ft_strdup(cmd[i]);
	while (i < len)
	if (arr[0])
	{
		if (len > 2)
		{
			arr[1] = ft_strdup(cmd[i]);
			if (!arr[1])
				return (NULL);
		}
		arr[len - 1] = 0;
		return (arr);
	}
	else
		ft_errno("malloc");
	return (NULL);
}

t_data *get_cmd(char **arr, char *paths, int file_in, int file_out)
{
	int index_path;
	char **path_arr;
	char *updated;
	t_data *node;

	path_arr = ft_split(paths, ':');
	updated = check_path(arr[0], path_arr, &index_path);
	if (path_arr && updated)
	{
		node = (t_data *) malloc (sizeof(t_data));
		if (node)
		{
			if (index_path != -1)
			{
				node->cmd_path = ft_strdup(path_arr[index_path]);
				if (!node->cmd_path)
					return (free(updated), free(node), NULL);
				node->cmd = ft_strjoin(node->cmd_path, updated);
			}
			else
			{
				node->cmd_path = NULL;
				node->cmd = ft_strdup(updated);
			}
			free_arr(&path_arr);
			if (!node->cmd)
						return (free(updated), free(node->cmd_path), free(node), NULL);
			node->arr_cmd = arr;
			if (!node->arr_cmd)
				return (free(node->cmd), free(updated), free(node), NULL);
			node->parent = 0;
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

void	add_back_list(t_data **lst, t_data *newlst)
{
	t_data	*tmp;

	if (!*lst)
		*lst = newlst;
	else
	{
		tmp = *lst;
		while (tmp -> next)
			tmp = tmp -> next;
		tmp -> next = newlst;
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

int has_special(char *str, int *start, int *end, char c)
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

char *ft_strdup_len(char *str, int len)
{
	char *cpy;
	int i;

	i = 0;
	if (str && !*str)
		return (ft_strdup(""));
	cpy = (char *) malloc (sizeof(char) * (len + 1));
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

int count_char(char *str, char c)
{
	int i;
	int count;

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

char **ft_special_split(char *s, char c, char condition)
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
		array[i] = NULL;
		while (*s && *s == c && *(s - 1) && *(s - 1) != condition)
			s++;
		while (*s)
		{
			len++;
			s++;
			if (*s == c && *(s - 1) != condition)
				break;
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
	trim = (char *) malloc (sizeof(char) * (end - start + 1) + 1);
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

void trim_array(char ***arr)
{
	int i;
	int j;
	char **arr_cpy;
	char *tmp;

	i = 0;
	j = 0;
	arr_cpy = *arr;

	while (arr_cpy[i])
	{
		tmp = arr_cpy[i];
		arr_cpy[j] = ft_strtrim(tmp, " ");
		if (!*arr_cpy[j])
		{
			free(arr_cpy[j]);
			arr_cpy[j] = NULL;
			j--;
		}
		free(tmp);
		tmp = NULL;
		j++;
		i++;
	}
	while (j < i)
		arr_cpy[j++] = 0;
}

char **arr_strdup (char **str)
{
	char **re;
	int i;

	if (!str)
		return (str);
	re = malloc (sizeof (char *) * (get_arr_len (str) + 1));
	if (!re)
		return (re);
	i = 0;
	while (str[i])
	{
		re[i] = ft_strdup (str[i]);
		i++;
	}
	re [i] = 0;
	return (re);
}

// it have to handle the spaces in the first args since it may contain two strings
char **handle_args(char **arr)
{
	int i;
	int j;
	int len;
	char **head;
	char **arr_return;

	i = 0;
	j = 1;
	head = ft_split(arr[i], ' ');
	len = get_arr_len(head);
	if (len > 1)
	{
		len += get_arr_len(arr + 1);
		arr_return = (char **) malloc (sizeof(char *) * (len + 1));
		while (head[i])
		{
			arr_return[i] = ft_strdup(head[i]);
			i++;
		}
		free_arr(&head);
		while (i < len)
			arr_return[i++] = ft_strdup(arr[j++]);
		arr_return[i] = 0;
		return (arr_return);
	}
	return (free_arr(&head), arr_strdup (arr));
}

int correct_commandes(char **argv, int len, t_data **head, char **env, int *fd)
{
    int i;
	char **arr;
	char **arr_tmp;
	t_data *node;
	char *path;

    i = 2;
	path = get_path(env);	//getting path through the path got by get_path
	if (!path)
		return (ft_errno("path invalid"), 0);
    while (i < len)
    {
        arr_tmp = ft_special_split(argv[i], '\'', '\\');

	
		trim_array(&arr_tmp);
		// read_arr (arr_tmp);
		arr = handle_args(arr_tmp);
		free_arr (&arr_tmp);
		if (!arr)
			return (ft_errno("malloc"), 0);
        node = get_cmd(arr, path, *fd, *(fd + 1));
		if (!node)
			return (free (path), free_list(head), 0);	//have to free all linked list (head & arr) & return ft_errno
		add_back_list(head, node);
        i++;
    }
	free (path);
	return (1);
}

int correct_files(char *file_in, char *file_out, int *fd_in, int *fd_out)
{
    *fd_in = open(file_in, O_RDONLY);
    *fd_out = open(file_out, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP);
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
	// read_arr((*cpy)->arr_cmd);
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
    if (ac == 5)
	{
        if (correct_files(av[1], av[ac - 1], &fd[0], &fd[1]))
		{
            if (correct_commandes(av, ac - 1, &head_cmd, env, fd))
			{
				processing_cmds(&head_cmd, env);
				write(1, "\033[32mSuccess\033[0m\n", 17);
			}
			else
				ft_errno("command failed");
		}
		else
			ft_errno("incorrect files");
	}
	else
		ft_errno("few arguments or more");
}
