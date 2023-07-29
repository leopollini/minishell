/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpollini <lpollini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/03 09:45:15 by lpollini          #+#    #+#             */
/*   Updated: 2023/07/19 14:19:21 by lpollini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*shft_strrchr(const char *s, char c, char ig1, char ig2)
{
	int		i;
	char	test;

	if (!s)
		return (NULL);
	i = 0;
	test = 0;
	while (s[i])
		i++;
	while (i--)
	{
		if (s[i] == ig1 && test != 2)
			test ^= 1;
		if (s[i] == ig2 && test != 1)
			test ^= 2;
		if (!test && s[i] == (char)c)
			return ((char *)(s + i));
	}
	if (s[i] == (char)c)
		return ((char *)(s + i));
	return (NULL);
}

char	*shft_strchr(const char *s, char c, char ig1, char ig2)
{
	int		i;
	char	test;

	if (!s)
		return (NULL);
	i = 0;
	test = 0;
	while (s[i])
	{
		if (s[i] == ig1 && test != 2)
			test ^= 1;
		if (s[i] == ig2 && test != 1)
			test ^= 2;
		if (!test && s[i] == (char)c)
			return ((char *)(s + i));
		i++;
	}
	if (s[i] == (char)c)
		return ((char *)(s + i));
	return (NULL);
}

char	*ft_strchrn(const char *s, char c, int n)
{
	int	i;

	if (!s)
		return (NULL);
	i = 0;
	while (s[i] && i < n)
	{
		if (s[i] == (char)c)
			return ((char *)(s + i));
		i++;
	}
	if (s[i] == (char)c)
		return ((char *)(s + i));
	return (NULL);
}

char	*ft_strchr(const char *s, char c)
{
	int	i;

	if (!s)
		return (NULL);
	i = 0;
	while (s[i])
	{
		if (s[i] == (char)c)
			return ((char *)(s + i));
		i++;
	}
	if (s[i] == (char)c)
		return ((char *)(s + i));
	return (NULL);
}
