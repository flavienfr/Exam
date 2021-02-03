/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inter.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froussel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 15:36:48 by froussel          #+#    #+#             */
/*   Updated: 2019/11/18 16:02:14 by froussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

int		c_in_set(char c, char *s)
{
	int i;

	i = -1;
	while (s[++i])
		if (s[i] == c)
			return (1);
	return (0);
}

void	ft_inter(char *s1, char *s2)
{
	char	str[128];
	int		i;
	int		idx;

	i = -1;
	idx = 0;
	*str = '\0';
	while (s1[++i])
		if (c_in_set(s1[i], s2) && !c_in_set(s1[i], str))
		{
			str[idx] = s1[i];
			str[idx + 1] = '\0';
			idx++;
		}
	i = -1;
	while (str[++i])
		ft_putchar(str[i]);
}

int		main(int ac, char **av)
{
	if (ac == 3)
		ft_inter(av[1], av[2]);
	ft_putchar('\n');
}
