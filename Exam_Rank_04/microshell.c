
#include "microshell.h"

int ft_strlen(char*str)
{
	int i = 0;
	while (str[i])
		i++;
	return (i);
}
void fd_putstr(char *str)
{
	write(2, str, ft_strlen(str));
}
void exit_fatal()
{
	fd_putstr("error: fatal\n");
	exit(EXIT_FAILURE);
}
char	*ft_strdup(char *s1)
{
	size_t	i;
	char	*ps;

	if (!(ps = malloc(sizeof(*ps) * (ft_strlen(s1) + 1))))
		exit_fatal();
	i = -1;
	while (s1[++i])
		ps[i] = s1[i];
	ps[i] = '\0';
	return (ps);
}

t_cmd	*create_cmd(char **av, int n_elem, t_cmd *cmd, int type)
{
	t_cmd *new_cmd;

	if (!(new_cmd = malloc(sizeof(*new_cmd))))
		exit_fatal();
	new_cmd->type = type;
	new_cmd->next = NULL;
	new_cmd->prev = cmd;
	if (cmd != NULL)
		cmd->next = new_cmd;
	if (!(new_cmd->args = (char **)malloc(sizeof(char *) * (n_elem + 1))))
		exit_fatal();
	int i = 0;
	for (; i < n_elem; ++i)
		new_cmd->args[i] = ft_strdup(av[i]);
	new_cmd->args[i] = NULL;
	return (new_cmd);
}

void clear(t_cmd *cmd)
{
	while (cmd)
	{
		int i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
		t_cmd *tmp = cmd;
		cmd = cmd->next;
		free(tmp);
	}
}

int exec_cd(t_cmd *cmd)
{
	int count = 0;
	int ret; 

	while (cmd->args[count])
		count++;
	if (count != 2)
	{
		fd_putstr("error: cd: bad arguments\n");
		return (1);
	}
	if ((ret = chdir(cmd->args[1])) < 0)
	{
		fd_putstr("error: cd: cannot change directory to ");
		fd_putstr(cmd->args[1]);
		fd_putstr("\n");
	}
	return (ret);
}

int non_builtin(t_cmd *cmd, char **env)
{
	pid_t pid;
	int status;
	int ret = 0;

	if (cmd->type == PIPE)
		if (pipe(cmd->pp) < 0)
			exit_fatal();
	pid = fork();
	if (pid < 0)
		exit_fatal();
	else if (pid == 0)
	{
		if (cmd->type == PIPE && (dup2(cmd->pp[1], 1)) < 0)
			exit_fatal();
		if (cmd->prev && cmd->prev->type == PIPE && (dup2(cmd->prev->pp[0], 0)) < 0)
			exit_fatal();
		if ((ret = execve(cmd->args[0], cmd->args, env)) < 0)
		{
			fd_putstr("error: cannot execute ");
			fd_putstr(cmd->args[0]);
			fd_putstr("\n");
		}
		exit (ret);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			ret = WEXITSTATUS(status);
		if (cmd->type == PIPE)
		{
			close(cmd->pp[1]);
			if (!cmd->next)
				close(cmd->pp[0]);
		}
		if (cmd->prev && cmd->prev->type == PIPE)
			close(cmd->prev->pp[0]);
	}
	return (ret);
}

int execute(t_cmd *cmd, char **env)
{
	int ret = 0;
	while (cmd)
	{
		if (!strcmp("cd", cmd->args[0]))
			ret = exec_cd(cmd);
		else
			ret = non_builtin(cmd, env);
		cmd = cmd->next;
	}
	return (ret);
}

int main(int ac, char **av, char **env)
{
	t_cmd *cmd = NULL;
	t_cmd *tmp = NULL;
	int type;
	int first = 1;
	int last = 0;
	while (++last < ac)
	{
		if (!strcmp(av[last], "|") || !strcmp(av[last], ";") || last + 1 == ac)
		{
			if (!strcmp(av[last], "|"))
				type = PIPE;
			else if (!strcmp(av[last], ";"))
				type = SEMI_COL;
			else
			{
				type = EMPTY;
				++last;
			}
			if (last - first != 0)
			{
				tmp = create_cmd(&av[first], last - first, tmp, type);
				if (cmd == NULL)
					cmd = tmp;
			}
			first = last + 1;
		}
	}
	int ret = execute(cmd, env);
	while (1);
	clear(cmd);
	return (ret);
}
