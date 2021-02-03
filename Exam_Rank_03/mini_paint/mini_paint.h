#ifndef MINI_PAINT
# define MINI_PAINT

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>

typedef struct	s_inf
{
	FILE	*file;
	int		w;
	int		h;
	char	bg;
	char	*map;
}				t_inf;

typedef struct	s_circ
{
	char			c;
	float			x;
	float			y;
	float			r;
	char 			car;
}				t_circ;

#endif
