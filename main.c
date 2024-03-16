#include <unistd.h>
#include <stdio.h>

typedef struct s_data{
    int fd_in;
    int fd_out;
    char *cmd_path;
    char *cmd_flags;
}   t_data;

int correct_commandes(char **argv, int len, t_data ***cmd)
{
    int i;

    i = 2;
    while (i < len)
    {
        //splitting commandes and get them into the struct each one of them
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
                        child_process();
                        //closing files
                    }
                    else if (pid != -1)
                    {
                        //second_command
                        parent_process();
                        //closing files
                    }
                }
            }
        }
    }
}