/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpollini <lpollini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/03 09:45:15 by lpollini          #+#    #+#             */
/*   Updated: 2023/07/23 12:11:14 by lpollini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup_len(const char *src, int len)
{
	unsigned int	i;
	char			*res;

	i = 0;
	if (!src)
		return (NULL);
	while (*(src + i) && i <= len)
		i++;
	res = malloc(i * 8 + 8);
	i = 0;
	while (*(src + i) && i <= len)
	{
		*(res + i) = *(src + i);
		i++;
	}
	*(res + i) = '\0';
	return (res);
}

char	*good_strdup(char *src)
{
	unsigned int	i;
	char			*res;

	i = 0;
	if (!src)
		return (NULL);
	while (*(src + i))
		i++;
	res = ft_calloc(i + 1, 8);
	i = 0;
	while (*(src + i))
	{
		while (*(src + i) == -1)
			src++;
		*(res + i) = *(src + i);
		if (!*(src + i))
			break ;
		i++;
	}
	*(res + i) = '\0';
	return (res);
}

char	*ft_strdup(const char *src)
{
	unsigned int	i;
	char			*res;

	i = 0;
	if (!src)
		return (NULL);
	while (*(src + i))
		i++;
	res = ft_calloc(i + 1, 8);
	i = 0;
	while (*(src + i))
	{
		*(res + i) = *(src + i);
		i++;
	}
	*(res + i) = '\0';
	return (res);
}
