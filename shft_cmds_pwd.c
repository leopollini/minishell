/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shft_cmds_pwd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpollini <lpollini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 20:34:40 by lpollini          #+#    #+#             */
/*   Updated: 2023/07/27 15:34:33 by lpollini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	shft_rmdirone(char *s, int lim)
{
	while (s[lim] != '/' && lim > 0)
		s[lim--] = -1;
	if (lim < 2)
		return (lim);
	while ((s[lim] == '/' || s[lim] == -1) && lim > 0)
		s[lim--] = -1;
	while (s[lim] != '/' && lim > 0)
		s[lim--] = -1;
	if (lim > 0)
		s[lim--] = -1;
	while (s[lim] == '/' && lim > 0)
		lim--;
	return (lim);
}

int	shft_pwd_better_1(char *res, char *nw, char **ori)
{
	if (access(res, F_OK) && *res)
	{
		ft_putstr_fd("cd: No such file or directory\n", STDERR_FILENO);
		free(nw);
		free(res);
		return (1);
	}
	free(*ori);
	free(nw);
	*ori = res;
	chdir(res);
	return (0);
}

int	shft_istab(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' \
		|| c == '\r' || c == '\v' || c == '\f')
		return (1);
	return (0);
}

int	shft_cmd_pwd(char *cmd, t_shell_stuff *sh)
{
	if (BLTINS)
		printf("PWD BUILTIN\n");
	printf("%s\n", shft_getenv("PWD", sh->envp, sh));
	return (0);
}
