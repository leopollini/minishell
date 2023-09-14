/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shft_cmds_echo_exit.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpollini <lpollini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 10:29:55 by lpollini          #+#    #+#             */
/*   Updated: 2023/09/14 16:09:59 by lpollini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	shft_cmd_env(char *cmd, t_shell_stuff *sh)
{
	int	i;

	if (BLTINS)
		printf("ENV BUILTIN\n");
	cmd += 3;
	while (shft_istab(*cmd))
		cmd++;
	if (*cmd)
	{
		ft_putstr_fd("env: too many arguments\n", STDERR_FILENO);
		return (127);
	}
	i = -1;
	while (++i <= sh->envn)
		if (sh->envp[i][0] && sh->envp[i][0] != '#')
			printf("%s\n", sh->envp[i]);
	return (0);
}

/*int	exit_ok(char *s, t_shell_stuff *sh)
{
	int	cs;

	cs = 0;
	while (shft_istab(*s))
		s++;
	if (!ft_strcmp_noend(s, "9223372036854775808") || \
		!ft_strcmp_noend(s, "-9223372036854775809"))
		cs = 1;
	while (*s && !shft_istab(*s))
	{
		if (!cs && (*s > '9' || *s < '0') && *s != '+' && *s != '-' && *s)
			cs = 1;
		s++;
	}
	while (shft_istab(*s))
		s++;
	if (*s)
		sh->exit_code = ft_putstr_fd("exit: too many arguments\n", \
					STDERR_FILENO) * 0 + 1;
	else if (cs)
		sh->exit_code = ft_putstr_fd("exit: numeric argument required\n", \
					STDERR_FILENO) * 0 + 2;
	else
		return (0);
	return (1);
}*/

int	ft_isnumber(const char *s)
{
	int	i;

	i = -1;
	while (s[++i])
		if (!(ft_isdigit(s[i]) || s[i] == '+' || s[i] == '-'))
			return (0);
	return (1);
}

int	exit_ok(char *s, t_shell_stuff *sh)
{
	char	**tb;
	int		i;

	while (shft_istab(*s))
		s++;
	tb = shft_split2(s, ' ', '\'', '\"');
	i = 0;
	while (tb[i])
		i++;
	if (i > 1)
		sh->exit_code = ft_putstr_fd("exit: too many arguments\n", \
					STDERR_FILENO) * 0 + 1;
	else if (!i)
		sh->exit_code = 0;
	else if (ft_isnumber(tb[0]) && (ft_strcmp_noend(tb[0], "9223372036854775808") && \
					ft_strcmp_noend(tb[0], "-9223372036854775809")))
		sh->exit_code = ft_atoi(tb[0]);
	else
		sh->exit_code = ft_putstr_fd("exit: numeric argument required\n", \
					STDERR_FILENO) * 0 + 2;
	while (i--)
		free(tb[i]);
	free(tb);
	return (1);
}

int	shft_cmd_exit(char *cmd, t_shell_stuff *sh)
{
	if (BLTINS)
		printf("EXIT BUILTIN\n");
	cmd += 4;
	printf("exit\n");
	exit_ok(cmd, sh);
	sh->doexit = 1;
	sh->lststatus = 0;
	return (0);
}

void	shft_echo_writer(char *cmd)
{
	char	test;

	test = 0;
	while (*cmd)
	{
		while (shft_istab(*cmd) && shft_istab(*(cmd + 1)) && !test)
			cmd++;
		if (*cmd == '\'' && test != 2)
			test ^= 1;
		else if (*cmd == '\"' && test != 1)
			test ^= 2;
		else if (shft_istab(*(cmd)) && !*(cmd + 1))
			return ;
		else
			write(1, cmd, 1);
		while (shft_istab(*cmd) && shft_istab(*(cmd + 1)) && !test)
			cmd++;
		cmd++;
	}
}

int	shft_cmd_echo(char *cmd, t_shell_stuff *sh)
{
	char	flag;
	int		i;

	if (BLTINS)
		printf("ECHO BUILTIN\n");
	flag = 0;
	cmd += 4;
	while (shft_istab(*cmd))
		cmd++;
	if (!shft_strcmp_noend2(cmd, "-n"))
		cmd += 2 + (flag++ & 0);
	while (shft_istab(*cmd))
		cmd++;
	shft_echo_writer(cmd);
	if (!flag)
		write(1, "\n", 1);
	return (0);
}
