/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shft_cmds_export_unset.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpollini <lpollini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 20:34:40 by lpollini          #+#    #+#             */
/*   Updated: 2023/10/08 11:56:56 by lpollini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	shft_cmd_unset_1(char *cmd, t_shell_stuff *sh, char ct)
{
	char	*temp;
	int		line;

	if (shft_strchr(cmd, ' ', '\'', '\"'))
	{
		temp = ft_strjoin("unset", shft_strchr(cmd, ' ', '\'', '\"'));
		shft_cmd_unset(temp, sh);
		free(temp);
	}
	temp = shft_strchr(cmd, ' ', '\'', '\"');
	if (temp)
	{
		ct = temp[0];
		temp[0] = '\0';
	}
	line = shft_getenv_line(cmd, sh->envp, sh);
	if (temp)
		temp[0] = ct;
	return (line);
}

/* UNSET builtin */
int	shft_cmd_unset(char *cmd, t_shell_stuff *sh)
{
	int		line;
	char	ct;

	if (BLTINS)
		printf("UNSET BUILTIN\n");
	cmd += 5;
	while (shft_istab(*cmd))
		cmd++;
	line = shft_cmd_unset_1(cmd, sh, ct);
	if (line > 0)
	{
		free(sh->envp[line]);
		sh->envp[line] = ft_strdup("###");
	}
	return (0);
}

int	shft_isallnum(char *s)
{
	while (*s <= '9' && *s >= '0')
		s++;
	if (*s != '=' && (shft_istab(*s) || !*s))
		return (1);
	return (0);
}

int	export_ok(char *s)
{
	int		i;
	char	*rest;

	i = 0;
	if (*s == '=' || shft_isallnum(s))
	{
		ft_putstr_fd("export: not a valid identifier\n", STDERR_FILENO);
		return (1);
	}
	while (ft_isalnum(*s))
		s++;
	if (*s != '=' && !shft_istab(*s) && *s)
	{
		ft_putstr_fd("export: not a valid identifier\n", STDERR_FILENO);
		return (1);
	}
	if (!*s || shft_istab(*s))
		return (0);
	return (-1);
}

int	export_lol(t_shell_stuff *sh)
{
	int	tempfd;

	tempfd = dup(STDOUT_FILENO);
	shft_execute_cmd(sh, "env | sort | awk \'$0=\"declare -x \"$0\' > .tempfile");
	dup2(tempfd, STDOUT_FILENO);
	shft_execute_cmd(sh, "cat .tempfile");
	shft_execute_cmd(sh, "rm .tempfile");
	return (0);
}

//* EXPORT builtin*/
int	shft_cmd_export(char *cmd, t_shell_stuff *sh)
{
	char	*temp[2];
	int		line;
	int		t;

	cmd += 6;
	while (shft_istab(*cmd))
		cmd++;
	if (BLTINS)
		printf("EXPORT BUILTIN\n");
	if (!*cmd)
		return (export_lol(sh));
	t = export_ok(cmd);
	if (t >= 0)
	{
		sh->lststatus = 1;
		return (t);
	}
	temp[0] = ft_strjoin("unset ", cmd);
	temp[1] = ft_strchr(temp[0], '=');
	ft_memmove(temp[1], "", 1);
	shft_cmd_unset(temp[0], sh);
	free(temp[0]);
	sh->envn++;
	temp[1] = sh->envp[sh->envn];
	sh->envp[sh->envn] = ft_strdup(cmd);
	free(temp[1]);
	return (0);
}
