/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_stuff.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpollini <lpollini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 15:33:19 by lpollini          #+#    #+#             */
/*   Updated: 2023/08/06 16:22:42 by lpollini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*build_put(t_shell_stuff *sh)
{
	char	*put;

	put = ft_strdup(GREENBOLD);
	put = ft_strjoin_free(put, sh->usr);
	put = ft_strjoin_free(put, "@");
	put = ft_strjoin_free(put, PROMPT);
	put = ft_strjoin_free(put, UNSET);
	put = ft_strjoin_free(put, ":");
	put = ft_strjoin_free(put, BLUEBOLD);
	return (put);
}

char	*better_pwd(char *pwd, const char *usr)
{
	char	*res;

	res = ft_strnstr(pwd, usr, ft_strlen(pwd));
	if (!res)
		return (ft_strdup(pwd));
	res = ft_strjoin("~", res + ft_strlen(usr));
	return (res);
}

char	*shft_prompt(t_shell_stuff *sh, int dofree)
{
	static char	*put;
	char		*tmp[2];

	if (dofree)
	{
		if (put)
			free(put);
		put = NULL;
		return (NULL);
	}
	if (!put)
		put = build_put(sh);
	tmp[1] = ft_strdup(put);
	tmp[0] = ft_strjoin_free_1(tmp[1], better_pwd(sh->pwd, sh->usr));
	free(tmp[1]);
	tmp[0] = ft_strjoin_free(tmp[0], UNSET);
	tmp[0] = ft_strjoin_free(tmp[0], "$ ");
	tmp[1] = readline(tmp[0]);
	free(tmp[0]);
	return (tmp[1]);
}
