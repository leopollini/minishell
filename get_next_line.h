/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpollini <lpollini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 11:03:52 by lpollini          #+#    #+#             */
/*   Updated: 2023/06/15 18:11:04 by lpollini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H

# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif
# define OPEN_MAX 
# define ALOT 99999

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>
# include <stdint.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include "get_next_line.h"

void	ft_clean(char *touch);
int		has_char(char *test, char a);
char	*ft_my_strdup(char *todo);
int		ft_insert(char *dest, char *soos, int *i);
int		ft_charge_buff(char	*toch, int fd);
char	*get_next_line(int fd);

#endif
