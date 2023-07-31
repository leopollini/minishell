/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_stuff.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 17:03:59 by lpollini          #+#    #+#             */
/*   Updated: 2023/07/30 22:06:55 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_env_free(char **envp, char *pwd, t_shell_stuff *sh)
{
	char	**tmp;
	char	*st;

	tmp = envp + shft_getenv_line("PWD", envp, sh);
	if (tmp < envp)
	{
		st = ft_strjoin("export PWD=", pwd);
		shft_cmd_export(st, sh);
		free(st);
		return ;
	}
	free(*tmp);
	*tmp = ft_strjoin("PWD=", pwd);
}

int	shft_getenv_line(char *find, char *envp[], t_shell_stuff *sh)
{
	char	*temp;
	int		i;

	i = -1;
	temp = ft_strjoin(find, "=");
	while (++i <= sh->envn)
	{
		if (!ft_strcmp_noend(envp[i], temp))
			break ;
		else if (!*envp[i])
		{
			i--;
			envp++;
		}
	}
	free(temp);
	if (i > sh->envn)
		return (-1);
	return (i);
}

char	*shft_getenv(char *find, char *envp[], t_shell_stuff *sh)
{
	char	*temp;
	int		i;

	i = -1;
	temp = ft_strjoin(find, "=");
	while (++i <= sh->envn)
		if (!ft_strcmp_noend(envp[i], temp))
			break ;
	free(temp);
	if (i > sh->envn)
		return ("");
	return (ft_strchr(envp[i], '=') + 1);
}

char	*shft_env_dup(char *in[], int i, int *r)
{
	static int	test;

	if (!test && in[i])
	{
		*r = i;
		return (ft_strdup(in[i]));
	}
	test++;
	return (ft_strdup(""));
}

int	shft_env_init_free(char *env[], char *in[], int sw)
{
	int	i;
	int	res;

	i = -1;
	if (sw)
	{
		while (++i < ENVN)
			free(env[i]);
		return (0);
	}
	while (++i < ENVN)
		env[i] = shft_env_dup(in, i, &res);
	return (res);
}
