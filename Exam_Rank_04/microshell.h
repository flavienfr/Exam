#ifndef MICROSHELL_H
# define MICROSHELL_H

# include <stdio.h>		//debug
# include <stdlib.h>	//malloc
# include <unistd.h>	//pipe
# include <string.h>	//strcmp

#include <sys/wait.h>

#define EMPTY 0
#define PIPE 1
#define SEMI_COL 2


typedef struct	s_cmd
{
	char			**args;
	int				type;
	int				pp[2];
	struct s_cmd	*next;
	struct s_cmd	*prev;
}				t_cmd;

#endif
