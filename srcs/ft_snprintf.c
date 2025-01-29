/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_snprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkurkar <mkurkar@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 19:33:34 by mkurkar           #+#    #+#             */
/*   Updated: 2025/01/29 02:55:50 by mkurkar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	ft_itoa_to_buf(int n, char *str, int *pos, size_t size)
{
	long	num;
	int		i;
	char	digits[12];

	i = 0;
	num = n;
	if (num < 0)
	{
		if (*pos < (int)size - 1)
			str[(*pos)++] = '-';
		num = -num;
	}
	if (num == 0)
	{
		if (*pos < (int)size - 1)
			str[(*pos)++] = '0';
		return ;
	}
	while (num > 0)
	{
		digits[i++] = '0' + (num % 10);
		num /= 10;
	}
	while (i > 0 && *pos < (int)size - 1)
		str[(*pos)++] = digits[--i];
}

static void	ft_strncpy_buf(const char *src, char *str, int *pos, size_t size)
{
	while (src && *src && *pos < (int)size - 1)
		str[(*pos)++] = *src++;
}

/**
 * @brief Custom implementation of snprintf for integer and string conversion
 * @param str Output buffer
 * @param size Buffer size
 * @param format Format string (supports %d and %s)
 * @param ... Variable arguments
 * @return Number of characters written (excluding null terminator)
 */
int	ft_snprintf(char *str, size_t size, const char *format, ...)
{
	va_list	args;
	int		pos;

	if (!str || !format || size == 0)
		return (0);
	va_start(args, format);
	pos = 0;
	while (*format && pos < (int)size - 1)
	{
		if (*format == '%')
		{
			format++;
			if (*format == 'd')
				ft_itoa_to_buf(va_arg(args, int), str, &pos, size);
			else if (*format == 's')
				ft_strncpy_buf(va_arg(args, char *), str, &pos, size);
			format++;
		}
		else
			str[pos++] = *format++;
	}
	str[pos] = '\0';
	va_end(args);
	return (pos);
}

#ifdef DEBUG

int	main(void)
{
	char	buf[64];
	int		n;

	n = ft_snprintf(buf, sizeof(buf), "Moves: %d, Player: %s", 42, "Hero");
	printf("Result (%d): '%s'\n", n, buf);
	n = ft_snprintf(buf, sizeof(buf), "%s: %d", "Score", -123);
	printf("Result (%d): '%s'\n", n, buf);
	return (0);
}
#endif
