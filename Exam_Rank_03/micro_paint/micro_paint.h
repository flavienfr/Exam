#ifndef MICRO_PAINT
# define MICRO_PAINT

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>

typedef struct	s_inf
{
	FILE	*file;
	int		w;
	int		h;
	char	bg;
	char	*map;
}				t_inf;

typedef struct	s_rect
{
	char			r;
	float			x;
	float			y;
	float			w;
	float			h;
	char 			c;
}				t_rect;

#endif
