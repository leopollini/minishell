/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 10:38:07 by naal-jen          #+#    #+#             */
/*   Updated: 2023/10/08 11:17:41 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"
#include "../minishell.h"

void	main_command_join(char **new_str, char **split)
{
	*new_str = ft_strjoin_free(*new_str, split[1]);
	*new_str = ft_strjoin_free(*new_str, " ");
}

void	initialize_three_to_zero(int *n, int *i, int *z)
{
	*n = 0;
	*i = 0;
	*z = 0;
}

void	pr_dir(char **new_str, char **split_wild, int first, int last_char)
{
	loco()->entry = readdir(loco()->dir);
	while (loco()->entry != NULL)
	{
		if (strcmp(loco()->entry->d_name, ".") == 0
			|| strcmp(loco()->entry->d_name, "..") == 0)
		{
			loco()->entry = readdir(loco()->dir);
			continue ;
		}
		if (match(loco()->entry->d_name, split_wild, first, last_char) != 0)
		{
			*new_str = ft_strjoin_free(*new_str, loco()->entry->d_name);
			*new_str = ft_strjoin_free(*new_str, " ");
			loco()->flag_no_process = 1;
		}
		loco()->entry = readdir(loco()->dir);
	}
}
