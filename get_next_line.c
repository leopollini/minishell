/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpollini <lpollini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 17:46:24 by lpollini          #+#    #+#             */
/*   Updated: 2023/07/19 14:07:37 by lpollini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	has_char(char *test, char a)
{
	while (*test)
		if (*(test++) == a)
			return (1);
	return (0);
}

char	*ft_my_strdup(char *todo)
{
	char	*res;
	int		i;

	i = 0;
	while (*(todo + i))
		i++;
	res = malloc(i * 8 + 8);
	*res = '\0';
	i = 0;
	while (todo[i] && todo[i] != -1)
	{
		res[i] = todo[i];
		i++;
	}
	res[i] = todo[i];
	return (res);
}

int	ft_insert(char *dest, char *soos, int *i)
{
	int	a;

	a = 0;
	while (soos[a] == -2)
	{
		soos++;
	}
	while (soos[a] && soos[a] != -1 && soos[a] != '\n')
	{
		dest[a] = soos[a];
		soos[a] = -2;
		a++;
	}
	if (soos[a] == '\n')
	{
		dest[a] = soos[a];
		soos[a] = -2;
		*i = *i + a + 1;
		return (1);
	}
	*i = *i + a;
	if (soos[a] == -1)
		return (1);
	return (0);
}

int	ft_charge_buff(char	*toch, int fd)
{
	int	i;

	i = read(fd, toch, BUFFER_SIZE);
	toch[BUFFER_SIZE] = 0;
	if (i == 0)
		return (1);
	return (0);
}

char	*get_next_line(int fd)
{
	static char	cbuff[BUFFER_SIZE + 1];
	char		res[ALOT];
	int			i;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
		return (NULL);
	i = -1;
	while (++i < ALOT)
		res[i] = '\0';
	i = 0;
	while (!has_char(res, '\n'))
	{
		if (ft_insert(res + i, cbuff, &i))
			break ;
		if (ft_charge_buff(cbuff, fd))
			break ;
	}
	if (*res)
		return (ft_my_strdup(res));
	return (NULL);
}
