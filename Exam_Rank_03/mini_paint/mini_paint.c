#include "mini_paint.h"

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

int		is_in(t_circ * circ, float x, float y)
{
	float	dist;
	
	dist = sqrtf((circ->x - x) * (circ->x - x) + (circ->y - y) * (circ->y - y));
	if (circ->r < dist)
		return (0);
	if (circ->r - dist < 1.00000000)
		return (2);
	return (1);
}

void	create_circ(t_inf *inf, t_circ *circ)
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
			isin = is_in(circ, (float)x, (float)y);
			if ((circ->c == 'C' && isin) || (circ->c == 'c' && isin == 2))
				inf->map[y * inf->w + x] = circ->car;
		}
	}
}

int		scan_map(t_inf *inf)
{
	t_circ	circ;
	int		ret;

	ret = 0;
	while ((ret = fscanf(inf->file, "%c %f %f %f %c\n",
			&circ.c, &circ.x, &circ.y, &circ.r, &circ.car)) == 5)
	{
		if (circ.c != 'c' && circ.c != 'C')
			return (1);
		if (circ.r <= 0)
			return (1);
		create_circ(inf, &circ);
	}
	if (ret != 5 || ret == -1)
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
	if (!(inf->map = (char*)malloc(sizeof(*inf->map) * count)))
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
	ret_clean(NULL, &inf, 0);
	return (0);
}
