/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_snprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkurkar <mkurkar@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 19:33:34 by mkurkar           #+#    #+#             */
/*   Updated: 2025/01/24 19:47:19 by mkurkar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void ft_itoa_to_buf(int n, char *str, int *pos, size_t size)
{
	long num;
	char digits[12]; // Enough for 32-bit int
	int i = 0;

	num = n;
	if (num < 0)
	{
		if (*pos < (int)size - 1)
			str[(*pos)++] = '-';
		num = -num;
	}

	// Handle 0 specially
	if (num == 0)
	{
		if (*pos < (int)size - 1)
			str[(*pos)++] = '0';
		return;
	}

	// Convert to digits in reverse order
	while (num > 0)
	{
		digits[i++] = '0' + (num % 10);
		num /= 10;
	}

	// Copy digits in correct order
	while (i > 0 && *pos < (int)size - 1)
		str[(*pos)++] = digits[--i];
}

/**
 * @brief Custom implementation of snprintf for move counter
 * @param str Output buffer
 * @param size Buffer size
 * @param format Format string
 * @return int Number of characters written
 */
int ft_snprintf(char *str, size_t size, const char *format, ...)
{
	va_list args;
	int pos;

	if (!str || !format || size == 0)
		return (0);

	va_start(args, format);
	pos = 0;

	while (*format && pos < (int)size - 1)
	{
		if (*format == '%' && *(format + 1) == 'd')
		{
			ft_itoa_to_buf(va_arg(args, int), str, &pos, size);
			format += 2;
		}
		else
			str[pos++] = *format++;
	}

	str[pos] = '\0';
	va_end(args);
	return (pos);
}
