#include <unistd.h>
#include <stdio.h>

//github example : https://github.com/gabcollet/pipex/tree/master
//github documentation : https://csnotes.medium.com/pipex-tutorial-42-project-4469f5dd5901
//read about env && access && execv


//we can use linked list that contains {fd_in, fd_out, cmd(argv[i]), next}


typedef struct s_data{
    int fd_in;
    int fd_out;
    char *cmd_path;
    char *cmd_flags;
}   t_data;

static int	get_words(char const *s, char c, int *index)
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

static int	ft_strsdup(char **array, const char *s, int len)
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

char	**free_arr(char ***array, int index)
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
			return (free_arr(&array, i));
	}
	array[words] = 0;
	return (array);
}

int correct_commandes(char **argv, int len, t_data ***cmd)
{
    int i;

    i = 2;
    while (i < len)
    {
        //splitting commandes and get them into the struct each one of them
        arr = ft_split(argv[i], ' ');
        execve
        i++;
    }
}

int correct_files(char *file_in, char *file_out)
{
    int fd_in;
    int fd_out;

    fd_in = open(file_in, O_RDONLY);
    fd_out = open (file_out, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP);
    close(fd_in);
    close(fd_out);
    if (fd_in != -1 && access(file_in, R_OK) != -1)
        if (fd_out != -1 && access(file_out, W_OK) != -1)
            return (1);
    return (0);
}

int main(int ac, char **av)
{
    int fd[2];
    t_pid pid;
    t_data **cmd;

    if (ac >= 5)
    {
        if (correct_files(av[1], av[ac - 1]))
        {
            if (correct_commandes(av, ac - 1, &cmd))
            {
                if (pipe(fd) != -1)
                {
                    pid = fork();
                    if (pid != -1 && !pid)
                    {
                        //first command
                        //child write
                        child_process();
                        //closing files
                    }
                    else if (pid != -1)
                    {
                        //second_command
                        //parent read
                        parent_process();
                        //closing files
                    }
                }
            }
        }
    }
}