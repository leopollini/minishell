/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpollini <lpollini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 20:30:22 by lpollini          #+#    #+#             */
/*   Updated: 2023/07/27 13:31:43 by lpollini         ###   ########.fr       */
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

	//implement $(cmd) why not??
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
	shft_pipexexec(piped, pipes - 1, sh);
	while (pipes--)
		free(piped[pipes]);
	free(piped);
	if (command != str)
		free(str);
	return (0);
}
