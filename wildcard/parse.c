/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 17:06:11 by naal-jen          #+#    #+#             */
/*   Updated: 2023/10/08 11:01:38 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"
#include "../minishell.h"

int	verify_match(char **split_wild, const char *str, int *i, int *j)
{
	char	*sub;
	int		sub_len;
	int		found;

	while (split_wild[(*j)])
	{
		sub = split_wild[(*j)];
		sub_len = ft_strlen(sub);
		found = 0;
		while (str[(*i)])
		{
			if (ft_strncmp(&str[(*i)], sub, sub_len) == 0)
			{
				(*i) += sub_len;
				found = 1;
				break ;
			}
			(*i)++;
		}
		if (!found)
			return (0);
		(*j)++;
	}
	return (1);
}

int	match(const char *str, char **split_wild, int first_char, int last_char)
{
	int		i;
	int		j;
	int		len;

	len = ft_strlen(str);
	i = 0;
	j = 0;
	if (first_char
		&& ft_strncmp(str, split_wild[0], ft_strlen(split_wild[0])) != 0)
		return (0);
	else if (first_char)
	{
		i += ft_strlen(split_wild[0]);
		j++;
	}
	if (!verify_match(split_wild, str, &i, &j))
		return (0);
	if (last_char
		&& ft_strncmp(&str[len - ft_strlen(split_wild[j - 1])],
			split_wild[j - 1],
			ft_strlen(&str[len - ft_strlen(split_wild[j - 1])])) != 0)
		return (0);
	return (1);
}

char	*wildcard_process(char *str, int *first, int *last, char ***split_wild)
{
	int		kind;
	char	**split;
	char	*new_str;

	if (!ft_strchr(str, '*'))
		return (ft_strdup(str));
	split = ft_split(str, ' ');
	if (ft_strlen_arr((void **)split) == 3)
		kind = 2;
	else
		kind = 1;
	if (!ft_strchr(split[kind], '*'))
	{
		ft_free_tab(split);
		return (str);
	}
	if (split[kind][0] != '*')
		*first = 1;
	if (split[kind][ft_strlen(split[1]) - 1] != '*')
		*last = 1;
	*split_wild = ft_split(split[kind], '*');
	new_str = ft_strjoin_free(ft_strdup(split[0]), " ");
	if (kind == 2)
		main_command_join(&new_str, split);
	return (new_str);
}

char	*check_for_wildcard_normal(char *str)
{
	char			**split_wild;
	char			*new_str;
	int				first_char;
	int				last_char;

	initialize_three_to_zero(&first_char, &last_char, &loco()->flag_no_process);
	new_str = wildcard_process(str, &first_char, &last_char, &split_wild);
	if (ft_strncmp(str, new_str, ft_strlen(str)) == 0)
		return (str);
	loco()->dir = opendir(".");
	if (!loco()->dir)
		return (NULL);
	pr_dir(&new_str, split_wild, first_char, last_char);
	closedir(loco()->dir);
	if (!loco()->flag_no_process)
	{
		free(new_str);
		return (str);
	}
	free(str);
	return (new_str);
}
