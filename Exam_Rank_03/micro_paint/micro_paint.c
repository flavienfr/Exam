#include "micro_paint.h"

int		ft_strlen(char *s)
{
	int i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int		ret_clean(char *str, t_inf *inf, int ret)
{
	if (str)
		write(1, str, ft_strlen(str));
	if (inf->map)
		free(inf->map);
	if (inf->file)
		fclose(inf->file);
	return (ret);
}

int		is_in(t_rect * rect, float x, float y)
{
	float	xbr;
	float	ybr;

	xbr = rect->x + rect->w;
	ybr = rect->y + rect->h;
	if (!(rect->x <= x && x <= xbr && rect->y <= y && y <= ybr))
		return (0);
	if (x - rect->x < 1.00000000 || xbr - x < 1.00000000)
		return (2);
	if (y - rect->y < 1.00000000 || ybr - y < 1.00000000)
		return (2);
	return (1);
}

void	create_rect(t_inf *inf, t_rect *rect)
{
	int		x;
	int		y;
	int		isin;

	y = -1;
	while (++y < inf->h)
	{
		x = -1;
		while (++x < inf->w)
		{
			isin = is_in(rect, (float)x, (float)y);
			if ((rect->r == 'R' && isin) || (rect->r == 'r' && isin == 2))
				inf->map[y * inf->w + x] = rect->c;
		}
	}
}

int		scan_map(t_inf *inf)
{
	t_rect	rect;
	int		ret;

	ret = 0;
	while ((ret = fscanf(inf->file, "%c %f %f %f %f %c\n",
			&rect.r, &rect.x, &rect.y, &rect.w, &rect.h, &rect.c)) == 6)
	{
		if (rect.r != 'r' && rect.r != 'R')
			return (1);
		if (rect.w <= 0 || rect.h <= 0)
			return (1);
		create_rect(inf, &rect);
	}
	if (ret != 6 || ret == -1)
		return (1);
	return (0);
}

int		scan_inf(t_inf *inf)
{
	int	count;
	int	i;

	if (fscanf(inf->file, "%d %d %c\n", &inf->w, &inf->h, &inf->bg) != 3)
		return (1);
	if (inf->w <= 0 || inf->w > 300)
		return (1);
	if (inf->h <= 0 || inf->h > 300)
		return (1);	
	i = -1;
	count = inf->w * inf->h;
	if (!(inf->map = (char *)malloc(sizeof(*inf->map) * count)))
		return (1);
	while (++i < count)
		inf->map[i] = inf->bg;
	return (0);
}

void	print_map(t_inf *inf)
{
	int		i;

	i = -1;
	while (++i < inf->h)
	{
		write(1, inf->map + i * inf->w, inf->w);
		write(1, "\n", 1);
	}
}

int		main(int ac, char **av)
{
	t_inf	inf;

	inf.map = NULL;
	inf.file = NULL;
	if (ac != 2)
		return (ret_clean("Error: argument\n", &inf, 1));
	if (!(inf.file = fopen(av[1], "r")))
		return (ret_clean("Error: Operation file corrupted\n", &inf, 1));
	if (scan_inf(&inf))
		return (ret_clean("Error: Operation file corrupted\n", &inf, 1));
	if (scan_map(&inf))
		return (ret_clean("Error: Operation file corrupted\n", &inf, 1));
	print_map(&inf);
	return (ret_clean(NULL, &inf, 0));
}
