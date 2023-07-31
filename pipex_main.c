/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpollini <lpollini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 12:46:25 by lpollini          #+#    #+#             */
/*   Updated: 2023/07/31 14:35:20 by lpollini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "minishell.h"

void	ft_free_tab(char **tb)
{
	int	i;

	i = 0;
	while (tb[i])
		free(tb[i++]);
	free(tb);
}

void	pipeft_exit(int arg)
{
	if (!(arg == WELLDONE))
	{
		write(2, "Error: ", 8);
		if (arg == PIPECREATERR)
			write(2, "Error during pipe creation!\n", 29);
		if (arg == NOINFILE)
			write(2, "Cannot find inpt!\n", 19);
		if (arg == NEOUGHINPT)
			write(2, "Invalid input!\n", 16);
		if (arg == ALLSENT)
			write(2, "Content was sent!\n", 19);
		if (arg == ALLRECIEVED)
			write(2, "Content was recieved!\n", 23);
		if (arg == NOOUTF)
			write(2, "Cant find out file!\n", 21);
		if (arg == CMDNOTFOUND)
			write(2, "Command not found!\n", 20);
		exit(127);
	}
	exit(0);
	return ;
}

char	*search_path(char *name, t_shell_stuff *sh)
{
	char	**temp;
	char	*otemp[2];
	int		i;

	i = 0;
	temp = ft_split(shft_getenv("PATH", sh->envp, sh), ':');
	otemp[1] = ft_strjoin("/", name);
	while (temp[i])
	{
		otemp[0] = ft_strjoin(temp[i], otemp[1]);
		if (open(otemp[0], 0) != -1)
			break ;
		free(otemp[0]);
		i++;
	}
	if (!temp[i])
		i = 0;
	ft_free_tab(temp);
	free(otemp[1]);
	if (i)
		return (otemp[0]);
	return (NULL);
}

static char	**shft_dupenv(t_shell_stuff *sh)
{
	char	**res;
	int		i;
	int		h;

	i = -1;
	h = 0;
	res = ft_calloc(sh->envn + 1, sizeof(char *) * 8);
	while (++i <= sh->envn)
		if (sh->envp[i][0] && sh->envp[i][0] != '#')
			res[h++] = ft_strdup(sh->envp[i]);
	res[h] = NULL;
	return (res);
}

int	command_fork(char **args, t_shell_stuff *sh, int doset)
{
	int		res;
	pid_t	fpid;
	int		pipefd[2];
	char	**envdp;

	pipe(pipefd);
	fpid = fork();
	if (fpid)
	{
		if (doset)
			dup2(pipefd[0], STDIN_FILENO);
		close (pipefd[1]);
		waitpid(fpid, &res, 0);
		return (res);
	}
	if (doset)
		dup2(pipefd[1], STDOUT_FILENO);
	close (pipefd[0]);
	envdp = shft_dupenv(sh);
	execve(args[0], args, envdp);
	ft_putstr_fd("Error: dorectory not a proper executable\n", STDERR_FILENO);
	sh->doexit = 1;
	ft_free_tab(envdp);
	return (0);
}

int	command(char *cmd, t_shell_stuff *sh, int doset)
{
	char		**args;
	char		*xpat;
	int			res;
	int			i;

	args = ft_split1(cmd, ' ', '\\');
	if (ft_strchr(args[0], '/') && access(args[0], X_OK) == 0)
		res = command_fork(args, sh, doset);
	else
	{
		xpat = search_path(args[0], sh);
		res = -1;
		if (xpat)
		{
			free(args[0]);
			args[0] = xpat;
			res = command_fork(args, sh, doset);
		}
	}
	ft_free_tab(args);
	return (res);
}

void	read_stdin(char *limiter)
{
	char	*buff;
	int		pipefd[2];

	pipe(pipefd);
	buff = (char *)1;
	while (buff)
	{
		buff = get_next_line(STDIN_FILENO);
		if (!ft_strncmp(buff, limiter, ft_strlen(limiter)))
			break ;
		ft_putstr_fd(buff, pipefd[1]);
		free(buff);
	}
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	free(buff);
}

int	shft_is_builtin(char *cd)
{
	int	i;

	if (shft_strcmp_noend2(cd, "pwd") && shft_strcmp_noend2(cd, "env") && \
		shft_strcmp_noend2(cd, "echo") && shft_strcmp_noend2(cd, "exit") && \
		shft_strcmp_noend2(cd, "cd") && shft_strcmp_noend2(cd, "unset") && \
		shft_strcmp_noend2(cd, "export"))
		return (-1);
	return (0);
}

char	*make_arg(char *s)
{
	int		i;
	char	*res;

	while (shft_istab(*s))
		s++;
	i = 0;
	while (!shft_istab(s[i]) && s[i])
		i++;
	res = malloc(8 * (i + 1));
	i = -1;
	while (!shft_istab(s[++i]) && s[i])
		res[i] = s[i];
	res[i] = '\0';
	if (ft_strchr(res, '>'))
	{
		return (NULL);
		free(res);
	}
	return (res);
}

int	shft_redir_inpt_1(char **cmd, char *arg, int temp)
{
	while (shft_istab(**cmd))
		*cmd = *cmd + 1;
	arg = make_arg(*cmd);
	temp = open(arg, O_RDONLY);
	if (temp != -1)
		dup2(temp, STDIN_FILENO);
	*cmd = *cmd + ft_strlen(arg);
	while (shft_istab(**cmd))
		*cmd = *cmd + 1;
	free(arg);
	if (temp != -1)
		dup2(temp, STDIN_FILENO);
	else
	{
		ft_putstr_fd("Error: file not found\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

int	shft_redir_inpt(char **cmd, t_shell_stuff *sh)
{
	char	*arg;
	int		temp;

	while (shft_istab(**cmd))
		*cmd = *cmd + 1;
	if (**cmd != '<')
		return (0);
	*cmd = *cmd + 1;
	if (**cmd == '<')
	{
		dup2(sh->tempfds[0], STDIN_FILENO);
		*cmd = *cmd + 1;
		while (shft_istab(**cmd))
			*cmd = *cmd + 1;
		arg = make_arg(*cmd);
		read_stdin(arg);
		*cmd = *cmd + ft_strlen(arg);
		while (shft_istab(**cmd))
			*cmd = *cmd + 1;
		free(arg);
		return (shft_redir_inpt(cmd, sh));
	}
	if (shft_redir_inpt_1(cmd, arg, temp))
		return (1);
	return (shft_redir_inpt(cmd, sh));
}

void	shft_clean_cmd(char *ori)
{
	int		i;
	int		h;

	i = -1;
	h = 0;
	while (ori[++i])
		if (ori[i] != -1)
			ori[h++] = ori[i];
	ori[h] = '\0';
}

int	shft_redir_outp_1(int temp, int pipefd[2], int *doset, char *cmd)
{
	if (temp != -1)
		dup2(temp, STDOUT_FILENO);
	else
	{
		ft_putstr_fd("Error: file not found\n", STDERR_FILENO);
		return (1);
	}
	pipe(pipefd);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[1]);
	temp = 0;
	*doset = 0;
	shft_clean_cmd(cmd);
	return (0);
}

int	shft_redir_outp_2(char **pos, char *cmd, int *flags, char **arg)
{
	*pos = shft_strchr(cmd, '>', '\"', '\'');
	if (!*pos)
		return (1);
	*pos = *pos + 1;
	if (**pos == '>')
	{
		*pos = *pos + 1;
		*flags = 02101;
	}
	else
		*flags = 01101;
	*arg = make_arg(*pos);
	if (!*arg)
	{
		*cmd = 0;
		return (1);
	}
	return (0);
}

int	shft_redir_outp(char *cmd, t_shell_stuff *sh, int *doset)
{
	char	*pos;
	char	*arg;
	int		pipefd[2];
	int		temp;
	int		flags;

	if (shft_redir_outp_2(&pos, cmd, &flags, &arg))
		return (0);
	temp = open(arg, flags, 0777);
	pos = shft_strchr(cmd, '>', '\"', '\'');
	flags = 0;
	while (flags < ft_strlen(arg) + 2)
	{
		if (pos[flags + 1] == '>')
			*(pos++) = -1;
		pos[flags] = -1;
		while (shft_istab(pos[flags]))
			pos++;
		flags++;
	}
	free(arg);
	if (shft_redir_outp_1(temp, pipefd, doset, cmd))
		return (1);
	return (shft_redir_outp(cmd, sh, doset));
}

static void	word_clean(char *str, int len)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (++i < len)
		if (str[i] != -1)
			str[j++] = str[i];
	str[j] = '\0';
}

void	shft_last_parse(char **s)
{
	int		test;
	char	*ori;

	ori = *s;
	test = 0;
	while (*ori)
	{
		if (*ori == -1)
			*ori = '>';
		if (*ori == -2)
			*ori = '<';
		if (*ori == '\'' && test != 2)
		{
			test ^= 1;
			*ori = -1;
		}
		if (*ori == '\"' && test != 1)
		{
			test ^= 2;
			*ori = -1;
		}
		ori++;
	}
	word_clean(*s, ft_strlen(*s));
}

int	shft_redirections(char **cmd, t_shell_stuff *sh, int *doset)
{
	if (shft_redir_inpt(cmd, sh))
		return (1);
	if (shft_redir_outp(*cmd, sh, doset))
		return (1);
	shft_last_parse(cmd);
	return (0);
}

void	builtin_temp_creat( void )
{
	int	filefd;

	filefd = open(FILENAME, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	dup2(filefd, STDOUT_FILENO);
	filefd = open(FILENAME, O_RDONLY);
	dup2(filefd, STDIN_FILENO);
}

int	builtin_cmds(char *cd, t_shell_stuff *sh, int doset)
{
	int	res;

	if (doset)
		builtin_temp_creat();
	res = 0x7fffffff;
	if (!shft_strcmp_noend2(cd, "pwd"))
		res = shft_cmd_pwd(cd, sh);
	if (!shft_strcmp_noend2(cd, "env"))
		res = shft_cmd_env(cd, sh);
	if (!shft_strcmp_noend2(cd, "echo"))
		res = shft_cmd_echo(cd, sh);
	if (!shft_strcmp_noend2(cd, "exit"))
		res = shft_cmd_exit(cd, sh);
	if (!shft_strcmp_noend2(cd, "cd"))
		res = shft_cmd_cd(cd, sh);
	if (!shft_strcmp_noend2(cd, "unset"))
		res = shft_cmd_unset(cd, sh);
	if (!shft_strcmp_noend2(cd, "export"))
		res = shft_cmd_export(cd, sh);
	if (res == 0x7fffffff)
		ft_putstr_fd("Error: make better cmd check lol\n", STDERR_FILENO);
	return (res);
}

int	shft_fr_to(char *cmd, t_shell_stuff *sh, int doset)
{
	int	pp[2];

	if (sh->doexit != -1 || shft_redirections(&cmd, sh, &doset))
		return (127);
	if (!*cmd)
	{
		ft_putstr_fd("Error: invalid redirection syntax\n", STDERR_FILENO);
		return (127);
	}
	if (shft_is_builtin(cmd) == 0)
		sh->lststatus = builtin_cmds(cmd, sh, doset);
	else
		sh->lststatus = command(cmd, sh, doset);
	if (sh->lststatus == -1)
	{
		sh->lststatus = 127;
		ft_putstr_fd("minishell: command \'", STDERR_FILENO);
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd("\' not found.\n", STDERR_FILENO);
		pipe(pp);
		close(pp[1]);
		dup2(pp[0], STDIN_FILENO);
	}
	return (sh->lststatus);
}

int	shft_pipexexec(char **cmds, int pipes, t_shell_stuff *sh)
{
	int	i;

	i = 0;
	if (pipes)
		shft_fr_to(cmds[i++], sh, 1);
	else
		shft_fr_to(cmds[i++], sh, 0);
	while (i < pipes)
		shft_fr_to(cmds[i++], sh, 1);
	dup2(sh->tempfds[1], STDOUT_FILENO);
	if (i == pipes)
		shft_fr_to(cmds[i], sh, 0);
	dup2(sh->tempfds[0], STDIN_FILENO);
	return (sh->lststatus);
}
