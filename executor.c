/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 20:30:22 by lpollini          #+#    #+#             */
/*   Updated: 2023/10/08 18:51:10 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	shft_isalnum(const int c)
{
	if ((c >= 'A' && c <= 'Z')
		|| (c >= 'a' && c <= 'z')
		|| (c >= '0' && c <= '9')
		|| c == '_')
		return (1);
	return (0);
}

char	*ft_strtab_join_free(char **s)
{
	char	*res;
	char	*temp;

	res = ft_strdup("");
	while (*s)
	{
		temp = res;
		res = ft_strjoin(res, *s);
		free(*s);
		free(temp);
		s++;
	}
	return (res);
}

char	*shft_arg(t_shell_stuff *sh, char *str)
{
	int	temp;

	temp = shft_atoi(str);
	if (temp < sh->argn)
		return (ft_strdup(sh->args[temp]));
	while (shft_isalnum(*str))
		str++;
	return (ft_strdup(str));
}

int	shft_istab1(char *str)
{
	while (*str == '\'' || *str == '\"')
		str++;
	if (shft_istab(*str) || !*str)
		return (1);
	return (0);
}

void	manage_dollar(char **str, t_shell_stuff *sh)
{
	char	ct;
	char	*st;
	char	*var;
	int		i;

	if ((*str)[0] == '?')
		var = ft_strjoin(ft_itoa(sh->lststatus), *str + 1);
	else if (shft_istab1((*str)))
		var = ft_strjoin("$", *str);
	else
	{
		i = 0;
		while ((*str)[i] && shft_isalnum((*str)[i]))
			i++;
		ct = (*str)[i];
		(*str)[i] = '\0';
		var = shft_getenv(*str, sh->envp, sh);
		(*str)[i] = ct;
		if (!*var)
			var = shft_arg(sh, *str);
		else
			var = ft_strjoin(var, *str + i);
	}
	free(*str);
	*str = var;
}

char	*parse_cmd(char *s, t_shell_stuff *sh)
{
	char	**dollard;
	char	*temp;
	int		i;
	int		d;

	dollard = shft_split(s, '$', '\0', '\'');
	d = 0;
	if (*s == '$')
		d = -1;
	while (dollard[++d])
		manage_dollar(dollard + d, sh);
	temp = ft_strtab_join_free(dollard);
	free(dollard);
	if (s[ft_strlen(s) - 1] == '$')
		temp = ft_strjoin_free(temp, "$");
	return (temp);
}

char	*manage_quotes(char *s)
{
	char	*res;
	int		i;
	int		j;

	i = -1;
	while (s[++i])
		shft_split1_test(s + i, '\'', '\"', 0);
	res = ft_calloc(i, 8);
	i = -1;
	j = 0;
	while (s[++i])
		if (s[i] != -1)
			res[j++] = s[i];
	free(s);
	return (res);
}

int	last_checks(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	if (!i)
		return (0);
	i--;
	while (shft_istab(str[i]))
		i--;
	if (str[i] == '|')
	{
		ft_putstr_fd("Pipe: bad pipe at EOL\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

int	shft_execute_cmd(t_shell_stuff *sh, char *str)
{
	char	**piped;
	int		pipes;
	char	*command;

	command = str;
	if (shft_strchr(str, '$', '\'', '\0'))
		str = parse_cmd(str, sh);
	pipes = 0;
	if (last_checks(str))
		return (127);
	piped = shft_split1(str, '|', '\'', '\"');
	if (!piped)
		return (127);
	while (piped[pipes])
		pipes++;
	if (piped[0])
		shft_pipexexec(piped, pipes - 1, sh);
	while (pipes--)
		free(piped[pipes]);
	free(piped);
	if (command != str)
		free(str);
	return (0);
}




int	shft_ch_checkok(char *cmd)
{
	return (0);
}

char	*shft_strchr_chain(char *s, char a)
{
	char	*t;

	t = s;
	s = shft_strchr(s, a, '\'', '\"');
	if  (s && s[0] && s[1] == a)
		return (s);
	if (s)
		return shft_strchr_chain(s + 1, '|');
	while (*t)
		t++;
	return (t);
}

char	*shft_strchr_s(char *s)
{
	char	*t;
	int		i;

	i = 0;
	while (*s)
	{
		if (*s == ')')
			i--;
		if (*s == '(')
			i++;
		if (!i)
			break ;
		s++;
	}
	if (!*s)
		return (NULL);
	return (s);
}

char	*shft_recursor(t_shell_stuff *sh, char *cmd)
{
	char	*t;

	t = cmd;
	while (shft_istab(*cmd))
		cmd++;
	if (*cmd != '(')
		return (t);
	t = cmd;
	cmd = shft_strchr_s(cmd);
	if (cmd)
		*cmd = '\0';
	else
		return (t + 1);
	shft_layer_rec(sh, t + 1);
	cmd++;
	while (shft_istab(*cmd))
		cmd++;
	if (!((*cmd == '|' && !sh->lststatus) || (*cmd == '&' && sh->lststatus)))
		shft_layer_rec(sh, cmd + 2);
	return (NULL);
}

void	shft_layer_rec(t_shell_stuff *sh, char *cmd)
{
	char	*cmdf;
	char	*t;
	char	dummy;

	cmd = shft_recursor(sh, cmd);
	if (!cmd || !*cmd)
		return ;
	cmdf = cmd;
	cmd = shft_strchr_chain(cmdf, '&');
	t = shft_strchr_chain(cmdf, '|');
	if ((unsigned long )t < (unsigned long )cmd)
		cmd = t;
	if (!*cmd)
		return ((void )shft_execute_cmd(sh, cmdf));
	*(cmd++) = '\0';
	shft_execute_cmd(sh, cmdf);
	if ((*cmd == '|' && !sh->lststatus) || (*cmd == '&' && sh->lststatus))
		return ;
	shft_layer_rec(sh, cmd + 1);
}

int	shft_chain_ops(t_shell_stuff *sh, char *cmd)
{
	if (shft_ch_checkok(cmd))
		return (sh->lststatus);
	shft_layer_rec(sh, cmd);
}
