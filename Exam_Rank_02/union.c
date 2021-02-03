/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   union.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: exam <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/19 20:49:37 by exam              #+#    #+#             */
/*   Updated: 2019/07/19 21:28:15 by exam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void 	ft_putchar(char c)
{
	write(1, &c, 1);
}

int		check(char c, char *str)
{
	int i;

	i = -1;
	while (str[++i])
		if (str[i] == c)
			return (0);
	return (1);
}

int		complete_str(char *s, char *str, int idx)
{
	int i;

	i = -1;
	while (s[++i])
	{
		if (check(s[i], str))
		{
			str[idx] = s[i];
			str[idx + 1] = '\0';
			idx++;
		}
	}
	return (idx);
}

void	ft_union(char *s1, char *s2)
{
	int idx;
	char str[128];

	idx = 0;
	*str = '\0';
	idx = complete_str(s1, str, idx);
	complete_str(s2, str, idx);
	while (str++)
		ft_putchar(*str);
}

int 	main(int ac, char **av)
{
	if (ac == 3)
		ft_union(av[1], av[2]);
	ft_putchar('\n');
	return (0);
}