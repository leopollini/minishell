/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shft_cmds_cd_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpollini <lpollini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 10:29:55 by lpollini          #+#    #+#             */
/*   Updated: 2023/08/06 14:57:33 by lpollini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	shft_pwd_better(char *nw, char **ori, t_shell_stuff *sh)
{
	char	temp[999];

	if (access(nw, F_OK) == -1)
	{
		ft_putstr_fd("cd: No such file or directory\n", STDERR_FILENO);
		sh->lststatus = 1;
		free(nw);
		return (1);
	}
	else
	{
		chdir(nw);
		getcwd(temp, 999);
		free(nw);
		nw = *ori;
		*ori = ft_strdup(temp);
		sh->lststatus = 0;
		free(nw);
		return (0);
	}
}

static char	cd_check(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[++i])
		if (shft_istab(cmd[i]) && cmd[i - 1] != '\\')
			break ;
	while (shft_istab(cmd[i]))
		i++;
	if (!cmd[i])
		return (0);
	return (1);
}

static char	*shft_cmd_cd_1(char *cmd, char *lstpwd, \
		t_shell_stuff *sh, char *newpwd)
{
	if (*cmd == '-')
	{
		cmd++;
		while (shft_istab(*cmd))
			cmd++;
		if (*cmd)
		{
			ft_putstr_fd("cd: too many arguments\n", STDERR_FILENO);
			return (NULL);
		}
		if (lstpwd)
			newpwd = ft_strdup(lstpwd);
		else
			newpwd = ft_strdup(sh->pwd);
		printf("%s\n", newpwd);
	}
	else if (*cmd == '~')
		newpwd = ft_strjoin(shft_getenv("HOME", sh->envp, sh), cmd + 1);
	else if (*cmd == '/')
		newpwd = ft_strdup(cmd);
	else if (*cmd)
		newpwd = ft_strjoin_by(sh->pwd, cmd, '/');
	return (newpwd);
}

static int	shft_cmd_cd_2(char *cmd, char **lstpwd, \
		t_shell_stuff *sh, char **newpwd)
{
	if (!*shft_getenv("PWD", sh->envp, sh))
	{
		*newpwd = ft_strjoin("export PWD=/", sh->pwd);
		shft_cmd_export(*newpwd, sh);
		free(*newpwd);
	}
	*newpwd = shft_cmd_cd_1(cmd, *lstpwd, sh, *newpwd);
	if (!*newpwd)
		return (1);
	if (*lstpwd)
		free(*lstpwd);
	*lstpwd = ft_strdup(sh->pwd);
	return (0);
}

/* CD builtin */
int	shft_cmd_cd(char *cmd, t_shell_stuff *sh)
{
	char		*newpwd;
	static char	*lstpwd;

	if (!cmd)
	{
		free(lstpwd);
		return (0);
	}
	cmd += 2;
	while (shft_istab(*cmd))
		cmd++;
	if (!*cmd)
		return (shft_cmd_cd("cd ~", sh));
	if (BLTINS)
		printf("CD BUILTIN\n");
	if (cd_check(cmd))
		return (ft_putstr_fd("cd: too many arguments\n", \
					STDERR_FILENO) * 0 + 1);
	if (shft_cmd_cd_2(cmd, &lstpwd, sh, &newpwd))
		return (1);
	return (shft_pwd_better(newpwd, &sh->pwd, sh));
}
