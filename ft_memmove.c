/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpollini <lpollini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/03 09:45:15 by lpollini          #+#    #+#             */
/*   Updated: 2023/07/13 13:52:18 by lpollini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t m)
{
	if (!dest)
		return (NULL);
	while (m--)
		*(char *)(dest + m) = *(char *)(src + m);
	return (dest);
}

void	*ft_memmove(void *dest, const void *src, size_t m)
{
	size_t	i;

	if (src < dest)
		return (ft_memcpy(dest, src, m));
	i = 0;
	while (i < m)
	{
		*(char *)(dest + i) = *(char *)(src + i);
		i++;
	}
	return (dest);
}
