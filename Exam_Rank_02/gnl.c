/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froussel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 16:31:58 by froussel          #+#    #+#             */
/*   Updated: 2019/11/18 18:23:11 by froussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//TEST---------
#include <fcntl.h>
#include <stdio.h>
//-------------
#include <stdlib.h>
#include <unistd.h>
#define BUF 100

int		ft_strlen(char *s)
{
	int i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_substr(char *s, int start, int len)
{
	char	*str;
	int		i;

	if (!s)
		return (NULL);
	if (start > ft_strlen(s))
		len = 0;
	else
		len -= start;
	if (!(str = malloc(sizeof(*str) * (len + 1))))
		return (NULL);
	i = 0;
	while (len-- > 0 && s[start + i])
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*str;
	char	*beg;
	int		len;

	if (!s1 && !s2)
		return (NULL);
	len = 0;
	if (s1)
		len = ft_strlen(s1);
	if (s2)
		len += ft_strlen(s2);
	if (!(str = malloc(sizeof(*str) * (len + 1))))
		return (NULL);
	beg = str;
	if (s1)
		while (*s1)
			*str++ = *s1++;
	if (s2)
		while (*s2)
			*str++ = *s2++;
	*str = '\0'; 
	return (beg);
}

int		free_all(char **s)
{
	if (*s)
	{
		free(*s);
		*s = NULL;
	}
	return (-1);
}

int		is_line(char *str)
{
	int i;

	i = -1;
	while (str[++i])
		if (str[i] == '\n')
			return (i);
	return (-1);
}

int		parse_line(char **str, char **line)
{
	char	*s;
	int 	ret;
	int		len;

	ret = 0;
	s = NULL;
	if (!*str || !**str)
	{
		if (!(*line = ft_substr("\0", 0, 0)))
			return (free_all(str));
	}
	else if ((len = is_line(*str)) >= 0)
	{
		if (!(*line = ft_substr(*str, 0, len)))
			return (free_all(str));
		if (!(s = ft_substr(*str, len + 1, ft_strlen(*str))))
			return (free_all(str));
		ret = 1;
	}
	else
	{
		if (!(*line = ft_substr(*str, 0, ft_strlen(*str))))
			return (free_all(str));
	}
	free_all(str);
	*str = s;
	return (ret);
	
}

int 	get_next_line(int fd, char **line)
{
	static char *str;
	char		buffer[BUF + 1];
	char		*tmp;
	int			ret;
	
	if (!line || fd < 0)
		return (free_all(&str));
	while ((ret = read(fd, buffer, BUF)) > 0)
	{
		buffer[ret] = 0;
		if (!(tmp = ft_strjoin(str, buffer)))
			return (free_all(&str));
		free_all(&str);
		str = tmp;
		if (is_line(str) >= 0)
			break;
	}
	if (ret < 0)
		return (free_all(&str));
	return (parse_line(&str, line));
}


//TEST--------------------------------
int		main(int ac, char **av)
{
	char	*line;
	int		fd;

	fd  = open(av[1], O_RDONLY);
	while (get_next_line(fd, &line) > 0)
	{
		printf("|%s|\n", line);
		free(line);
	}
	printf("|%s|\n", line);
	free(line);
	return (0);
}
//-------------------------------------
