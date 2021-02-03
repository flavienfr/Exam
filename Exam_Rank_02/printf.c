#include "ft_printf.h"

int 	ft_putchar(char c)
{
	write(1, &c, 1);
	return (1);
}

int 	ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int		c_in_set(char c, char *set)
{
	int i;

	i = -1;
	while (set[++i])
		if (set[i] == c)
			return (1);
	return (0);
}

int 	fill_in(char c, int len)
{
	int i = -1;

	while (++i < len)
		ft_putchar(c);
	return (i);
}

void	ft_putnbr(long nb)
{
	if (nb < 0)
		nb *= -1;
	if (nb >= 10)
	{
		ft_putnbr(nb / 10);
		ft_putnbr(nb % 10);
	}
	else
		ft_putchar(nb + '0');
}

void 	dec_to_hex(long int dec)
{
	char	shex[20];
	int		i;

	i = 0;
	while (dec)
	{
		shex[i++] = "0123456789abcdef"[dec % 16];
		dec = dec / 16;
	}
	if (i == 0)
		shex[i++] = '0';
	while (--i >= 0)
		ft_putchar(shex[i]);
}

//------------------------------------------------------------------
int		s_flag(int width, int prec, char *str, int len, int is_prec)
{
	int 	size;
	int 	i = -1;

	if (!str)
		str = "(null)";
	size = ft_strlen(str);
	if (is_prec && prec >= 0 && prec < size)
		size = prec;
	if (width > size)
		len += fill_in(' ', width - size);
	while (++i < size)
		len += ft_putchar(str[i]);
	return (len);
}

int 	d_flag(int width, int prec, long nb, int len)
{
	int size = 1;
	long tmp;

	tmp = nb;
	tmp = (tmp < 0) ? -tmp : tmp;
	while ((tmp = tmp / 10) > 0)
		size++;
	if (nb < 0)
		width--;
	if (width > size && size >= prec)
		len += fill_in(' ', width - size);
	else if (width > prec && prec > size)
		len += fill_in(' ', width - prec);
	if (nb < 0)
		len += ft_putchar('-');
	if (prec > size)
		len += fill_in('0', prec - size);
	ft_putnbr(nb);
	len += size;
	return (len);
}

int 	x_flag(int width, int prec, unsigned int nb, int len, int is_prec)
{
	int				size = 1;
	unsigned int	tmp;
	
	tmp = nb;
	while ((tmp = tmp / 16) > 0)
		size++;
	if (width > size && size >= prec)
		len += fill_in(' ', width - size);
	else if (width > prec && prec > size)
		len += fill_in(' ', width - prec);
	if (prec > size)
		len += fill_in('0', prec - size);
	if (nb == 0  && prec == 0 && is_prec)
		return (len);
	dec_to_hex(nb);
	len += size;
	return (len);
}

int		parser(const char *format, va_list ap)
{
	int i = -1;
	int len = 0;
	int width;
	int prec;
	int is_prec;

	while (format[++i])
	{
	width = 0;
	prec = 0;
	is_prec = 0;
		if (format[i] == '%')
		{
			i++;
			while (c_in_set(format[i], "0123456789"))
				width = (width * 10) + (format[i++] - '0');
			if (format[i] == '.')
			{
				is_prec = 1;
				i++;
			}
			while (c_in_set(format[i], "0123456789"))
				prec = (prec * 10) + (format[i++] - '0');
			if (format[i] == 's')
				len = s_flag(width, prec, va_arg(ap, char *), len, is_prec);
			if (format[i] == 'd')
				len = d_flag(width, prec, va_arg(ap, int), len);
			if (format[i] == 'x')
				len = x_flag(width, prec, va_arg(ap, unsigned int), len, is_prec);
		}
		else
			len += ft_putchar(format[i]);
	}
	return (len);
}

int		ft_printf(const char *format, ...)
{
	int		len;
	va_list	ap;

	if (!format)
		return (-1);
	va_start(ap, format);
	len = parser(format, ap);
	va_end(ap);
	return (len);
}
