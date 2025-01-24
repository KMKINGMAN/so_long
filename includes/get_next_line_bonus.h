/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkurkar <mkurkar@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:35:46 by mkurkar           #+#    #+#             */
/*   Updated: 2025/01/24 19:43:19 by mkurkar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# ifndef FD_MAX
#  define FD_MAX FOPEN_MAX
# endif

# if FD_MAX > FOPEN_MAX
#  undef FD_MAX
#  define FD_MAX FOPEN_MAX
# endif

# if FD_MAX < 1
#  undef FD_MAX
#  define FD_MAX FOPEN_MAX
# endif

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE BUFSIZ
# endif

# if BUFFER_SIZE > 1000000
#  undef BUFFER_SIZE
#  define BUFFER_SIZE 1000000
# endif

# if BUFFER_SIZE < 1
#  undef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

char			*get_next_line(int fd);
int				ft_gnl_strlen(char *str);
int				ft_gnl_strchr(char *str, char ch);
char			*ft_gnl_strdup(char *original);
char			*ft_gnl_strjoin(char *s1, char *s2);
char			*ft_gnl_substr(char *str, int start, int max);

#endif
