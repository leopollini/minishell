/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 12:46:25 by lpollini          #+#    #+#             */
/*   Updated: 2023/10/08 18:51:00 by naal-jen         ###   ########.fr       */
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

int	shft_putter(char *s1, char *s2, char *s3, int fd)
{
	ft_putstr_fd(s1, fd);
	ft_putstr_fd(s2, fd);
	ft_putstr_fd(s3, fd);
	return (0);
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
		return (WEXITSTATUS(res));
	}
	if (doset)
		dup2(pipefd[1], STDOUT_FILENO);
	envdp = shft_dupenv(sh);
	close (pipefd[0]);
	execve(args[0], args, envdp);
	shft_putter("minishell: \'", args[0], "\': Is a directory\n", STDERR_FILENO);
	sh->doexit = 1;
	sh->exit_code = 126;
	ft_free_tab(envdp);
	return (0);
}

int	command(char *cmd, t_shell_stuff *sh, int doset)
{
	char		**args;
	char		*xpat;
	int			res;
	int			i;

	args = shft_split2(cmd, ' ', '\'', '\"');
	if (access(args[0], F_OK | R_OK) == -1 && access(args[0], F_OK) == 0)
		return (126 + shft_putter("minishell: \'", args[0], "\': Permission denied\n", STDERR_FILENO) * 0);
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

char	*shft_get_word(char *in)
{
	char	test;
	char	*res;
	int		i;

	test = 0;
	i = 0;
	res = malloc(sizeof(char ) * 999);
	while (shft_istab(*in))
		in++;
	while (*in && (!shft_istab(*in) || test))
	{
		if (*in == '\'' && test != 2)
			test ^= 1;
		else if (*in == '\"' && test != 1)
			test ^= 2;
		else
			res[i++] = *in;
		in++;
	}
	res[i] = '\0';
	return (res);
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

void	clean_stuff(char *s, int l)
{
	*(s++) = -1;
	while (shft_istab(*s) || *s == '\'' || *s == '\"')
		*(s++) = -1;
	while (*s == '\'' || *s == '\"' || l--)
		*(s++) = -1;
}

int	read_stdin(char *limiter)
{
	char	*buff;
	int		pipefd[2];
	char	*comp;
	int		i;

	comp = shft_get_word(limiter + 2);
	pipe(pipefd);
	buff = (char *)1;
	while (buff)
	{
		buff = get_next_line(STDIN_FILENO);
		if (!shft_strcmp_noend(buff, comp))
			break ;
		ft_putstr_fd(buff, pipefd[1]);
		free(buff);
	}
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	free(buff);
	free(comp);
	limiter[0] = -1;
	clean_stuff(limiter + 1, ft_strlen(comp));
	return (0);
}

int	shft_redir_inpt(char *cmd, t_shell_stuff *sh)
{
	char	*p;
	char	*filename;
	int		tempfd;

	p = shft_strchr(cmd, '<', '\'', '\"');
	if (!p)
	{
		word_clean(cmd, ft_strlen(cmd));
		return (0);
	}
	if (*(p + 1) == '<')
		return (read_stdin(p));
	filename = shft_get_word(p + 1);
	tempfd = open(filename, O_RDONLY);
	clean_stuff(p, ft_strlen(filename));
	if (access(filename, R_OK) == -1 && access(filename, F_OK) != -1)
		return (shft_putter("minishell: \'", filename, "\': Permission denied\n", STDERR_FILENO) + 1);
	if (tempfd == -1)
	{
		shft_putter("minishell: \'", filename, "\': No such file or directory\n", STDERR_FILENO);
		word_clean(cmd, ft_strlen(cmd));
		return (1);
	}
	dup2(tempfd, STDIN_FILENO);
	return (shft_redir_inpt(cmd, sh));
}

int	manage_redir_o(char *filename, int tempfd, char *p, int append)
{
	if (append)
		*p = -1;
	clean_stuff(p + append, ft_strlen(filename));
	if (access(filename, W_OK) == -1 && access(filename, F_OK) != -1)
	{
		shft_putter("minishell: \'", filename, "\': Permission denied\n", STDERR_FILENO);
		return (1);
	}
	dup2(tempfd, STDOUT_FILENO);
	return (0);
}

int	shft_redir_outpt(char *cmd, t_shell_stuff *sh, int *doset)
{
	char	*p;
	char	*filename;
	int		tempfd;
	int		append;

	p = shft_strchr(cmd, '>', '\'', '\"');
	if (!p)
	{
		word_clean(cmd, ft_strlen(cmd));
		return (0);
	}
	*doset = 0;
	append = 0;
	if (*(p + 1) == '>')
		append = 1;
	filename = shft_get_word(p + 1 + append);
	if (append)
		tempfd = open(filename, 02101, 0666);
	else
		tempfd = open(filename, 01101, 0666);
	if (append)
		*p = -1;
	if (manage_redir_o(filename, tempfd, p, append))
		return (1);
	return (shft_redir_outpt(cmd, sh, doset));
}

void	shft_last_parse_1(char **s)
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
		ori++;
	}
	word_clean(*s, ft_strlen(*s));
}

int	shft_redirections(char **cmd, t_shell_stuff *sh, int *doset)
{
	if (shft_redir_inpt(*cmd, sh) || shft_redir_outpt(*cmd, sh, doset))
		return (1);
	shft_last_parse_1(cmd);
	return (0);
}

void	builtin_temp_creat( void )
{
	int	filefd;

	filefd = open(FILENAME, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	dup2(filefd, STDOUT_FILENO);
	filefd = open(FILENAME, O_RDONLY);
	dup2(filefd, STDIN_FILENO);
}

/*void	shft_last_parse_2(char **s)
{
	int		test;
	char	*ori;

	ori = *s;
	test = 0;
	while (*ori)
	{
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
}*/

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

int	builtin_cmds(char *cd, t_shell_stuff *sh, int doset)
{
	int	res;

	if (doset)
		builtin_temp_creat();
	res = 0x7fffffff;
	if (!shft_strcmp_noend2(cd, "echo"))
		res = shft_cmd_echo(cd, sh);
	if (!shft_strcmp_noend2(cd, "export"))
		res = shft_cmd_export(cd, sh);
	shft_last_parse_1(&cd);
	if (!shft_strcmp_noend2(cd, "pwd"))
		res = shft_cmd_pwd(cd, sh);
	if (!shft_strcmp_noend2(cd, "env"))
		res = shft_cmd_env(cd, sh);
	if (!shft_strcmp_noend2(cd, "exit"))
		res = shft_cmd_exit(cd, sh);
	if (!shft_strcmp_noend2(cd, "cd"))
		res = shft_cmd_cd(cd, sh);
	if (!shft_strcmp_noend2(cd, "unset"))
		res = shft_cmd_unset(cd, sh);
	if (res == 0x7fffffff)
		ft_putstr_fd("Error: make better cmd check lol\n", STDERR_FILENO);
	return (res);
}

void	non_executable_handler(char *cmd, t_shell_stuff *sh)
{
	if (sh->lststatus == 126)	
		return ;
	sh->lststatus = 127;
	if (shft_strchr(cmd, '/', '\'', '\"') && access(cmd, F_OK) == -1)
		shft_putter("minishell: \'", cmd, "\': No such file or directory\n", STDERR_FILENO);
	else
		shft_putter("minishell: \'", cmd, "\': command not found\n", STDERR_FILENO);
}

//* ---------------------------------- nizz ---------------------------------- */

int	check_for_bonus(char *cmd)
{
	int		i;
	int		flag;
	char	fs;

	i = -1;
	flag = 0;
	fs = 0;
	while (cmd[++i])
	{
		if (cmd[i] == '\'' || fs != 1)
			fs ^= 2;
		if (cmd[i] == '\"' || fs != 2)
			fs ^= 1;
		if (fs)
			continue ;
		if (cmd[i] == '&' && cmd[i + 1] == '&')
		{
			flag = 1;
			loco()->n++;
		}
		else if (cmd[i] == '|' && cmd[i + 1] == '|')
		{
			flag = 1;
			loco()->n++;
		}
	}
	if (flag)
		return (1);
	return (0);
}

char *clean_cmd(char *str)
{
	int		i;
	int		j;
	char	*new_str;

	i = 0;
	j = 0;
	new_str = NULL;
	new_str = (char *)ft_calloc(ft_strlen(str), sizeof(char));
	if (!new_str)
		return (NULL);
	while(str[i])
	{
		if (str[i] == '(' || str[i] == ')')
			i++;
		new_str[j++] = str[i++]; 
	}
	free(str);
	return (new_str);
}

int	execution_proccess_and_bonus(int *pp, t_shell_stuff *sh, int doset)
{
	char	**cmds;
	int		counter;
	int		fixer;

	counter = -1;
	fixer = 0;
	cmds = ft_split_operators(loco()->piece);
	if (cmds[0][0] == '1')
	{
		ft_free_tab(cmds);
		return (sh->lststatus);
	}
	else if (cmds[0][0] == '0')
	{
		if (sh->doexit != -1 || shft_redirections(&cmds[1], sh, &doset))
		{
			pipe(pp);
			close(*(pp + 1));
			dup2(*pp, STDIN_FILENO);
			return (1, sh->lststatus = 1);
		}
		if (shft_is_builtin(cmds[1]) == 0)
			sh->lststatus = builtin_cmds(cmds[1], sh, doset);
		else
			sh->lststatus = command(cmds[1], sh, doset);
		if (sh->lststatus == -1)
		{
			non_executable_handler(cmds[1], sh);
			pipe(pp);
			close(*(pp + 1));
			dup2(*pp, STDIN_FILENO);
		}
	}
	else
	{
		while (++counter < 2)
		{
			//* the following code is not going to work correctly you need another logic
			if (counter == 0 && doset == 1)
			{
				doset = 0;
				fixer = 1;
			}
			else if (counter == 1 && fixer == 1 && loco()->n == 0)
			{
				doset = 1;
				fixer = 0;
			}
			if (ft_strchr(cmds[0], '('))
				cmds[0] = clean_cmd(cmds[0]);
			//*	maybe here you check in general for the lststatus and on that handle the execution of the command
			// if (counter == 1 && (sh->lststatus == 1 || sh->lststatus == 127 || sh->lststatus == 126))
			// 	break ;
			if (sh->lststatus == 1 || sh->lststatus == 127 || sh->lststatus == 126)
				break ;
			else if (counter == 1 && loco()->parentheses == 1)
				break ;
			if (sh->doexit != -1 || shft_redirections(&cmds[counter], sh, &doset))
			{
				pipe(pp);
				close(*(pp + 1));
				dup2(*pp, STDIN_FILENO);
				return (1, sh->lststatus = 1);
			}
			if (shft_is_builtin(cmds[counter]) == 0)
				sh->lststatus = builtin_cmds(cmds[counter], sh, doset);
			else
				sh->lststatus = command(cmds[counter], sh, doset);
			if (sh->lststatus == -1)
			{
				non_executable_handler(cmds[counter], sh);
				pipe(pp);
				close(*(pp + 1));
				dup2(*pp, STDIN_FILENO);
			}
		}
	}
	return (sh->lststatus);
}

int	execution_proccess_or_bonus(int *pp, t_shell_stuff *sh, int doset)
{
	char	**cmds;
	int		counter;
	int		fixer;

	counter = -1;
	fixer = 0;
	cmds = ft_split_operators(loco()->piece);
	if (cmds[0][0] == '0')
	{
		ft_free_tab(cmds);
		return (sh->lststatus);
	}
	else if (cmds[0][0] == '1')
	{
		if (sh->doexit != -1 || shft_redirections(&cmds[1], sh, &doset))
		{
			pipe(pp);
			close(*(pp + 1));
			dup2(*pp, STDIN_FILENO);
			return (1, sh->lststatus = 1);
		}
		if (shft_is_builtin(cmds[1]) == 0)
			sh->lststatus = builtin_cmds(cmds[1], sh, doset);
		else
			sh->lststatus = command(cmds[1], sh, doset);
		if (sh->lststatus == -1)
		{
			non_executable_handler(cmds[1], sh);
			pipe(pp);
			close(*(pp + 1));
			dup2(*pp, STDIN_FILENO);
		}
	}
	else
	{
		while (++counter < 2)
		{
			if (counter == 0 && doset == 1)
			{
				doset = 0;
				fixer = 1;
			}
			else if (counter == 1 && fixer == 1 && loco()->n == 0)
			{
				doset = 1;
				fixer = 0;
			}
			if (ft_strchr(cmds[0], '('))
				cmds[0] = clean_cmd(cmds[0]);
			if (counter == 1 && sh->lststatus == 0)
				break ;
			else if (counter == 1 && loco()->parentheses == 1)
				break ;
			else if (counter == 0 && (sh->lststatus == 1 || sh->lststatus == 127 || sh->lststatus == 126))
				continue ;
			if (sh->doexit != -1 || shft_redirections(&cmds[counter], sh, &doset))
			{
				pipe(pp);
				close(*(pp + 1));
				dup2(*pp, STDIN_FILENO);
				return (1, sh->lststatus = 1);
			}
			if (shft_is_builtin(cmds[counter]) == 0)
				sh->lststatus = builtin_cmds(cmds[counter], sh, doset);
			else
				sh->lststatus = command(cmds[counter], sh, doset);
			if (sh->lststatus == -1)
			{
				non_executable_handler(cmds[counter], sh);
				pipe(pp);
				close(*(pp + 1));
				dup2(*pp, STDIN_FILENO);
			}
		}
	}
	return (sh->lststatus);
}

char *command_cleaner_and(char *tmp)
{
	int		i;
	int		j;
	char	*new;

	i = 0;
	j = 0;
	while (tmp[i] && (tmp[i] == ' ' || tmp[i] == '&'))
		i++;
	new = ft_calloc(ft_strlen(tmp) - i + 1, sizeof(char));
	if (!new)
		return (NULL);
	while (tmp[i])
	{
		new[j] = tmp[i];
		j++;
		i++;
	}
	free(tmp);
	return (new);
}

void	check_for_operator(char *cmd)
{
	int	i;

	i = -1;
	while (cmd[++i])
	{
		if (cmd[i] == '&' && cmd[i + 1] == '&')
			loco()->and = 1;
		else if (cmd[i] == '|' && cmd[i + 1] == '|')
			loco()->or = 1;
	}
	return ;
}

char *cmd_cleaner(char *tmp, int index, t_shell_stuff *sh)
{
	char	*new_tmp;
	int		i;

	i = 0;
	new_tmp = (char *)ft_calloc(ft_strlen(tmp) - index + 2, sizeof(char));
	if (!new_tmp)
		return (NULL);
	if (sh->lststatus)
		new_tmp[i++] = '1';
	else
		new_tmp[i++] = '0';
	while (tmp[index])
	{
		new_tmp[i++] = tmp[index++];
	}
	free(tmp);
	return (new_tmp);
}

char *cmd_parentheses_and_cleaner(char *cmd, int first_para, int last_para, t_shell_stuff *sh)
{
	char	*new_cmd;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (sh->lststatus == 1 || sh->lststatus == 127 || sh->lststatus == 126)
	{
		new_cmd = (char *)ft_calloc(ft_strlen(cmd) - last_para + 2, sizeof(char));
		if (!new_cmd)
			return (NULL);
		new_cmd[j++] = '1';
		last_para++;
		while (cmd[last_para])
			new_cmd[j++] = cmd[last_para++];
		free(cmd);
		return (new_cmd);
	}
	while (cmd[i] && (cmd[i] != '&' && cmd[i] != '|')) //! might not work 
		i++;
	new_cmd = (char *)ft_calloc(ft_strlen(cmd) - i + 2, sizeof(char));
	if (!new_cmd)
		return (NULL);
	new_cmd[j++] = '0';
	while (cmd[i])
	{
		if (i == first_para)
			i++;
		else if (i == last_para)
			i++;
		new_cmd[j++] = cmd[i++];
	}
	free(cmd);
	return (new_cmd);
}

char *cmd_parentheses_or_cleaner(char *cmd, int first_para, int last_para, t_shell_stuff *sh)
{
	char	*new_cmd;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (sh->lststatus == 0)
	{
		new_cmd = (char *)ft_calloc(ft_strlen(cmd) - last_para + 2, sizeof(char));
		if (!new_cmd)
			return (NULL);
		new_cmd[j++] = '0';
		last_para++;
		while (cmd[last_para])
			new_cmd[j++] = cmd[last_para++];
		free(cmd);
		return (new_cmd);
	}
	while (cmd[i] && (cmd[i] != '&' && cmd[i] != '|')) //! might not work 
		i++;
	new_cmd = (char *)ft_calloc(ft_strlen(cmd) - i + 2, sizeof(char));
	if (!new_cmd)
		return (NULL);
	new_cmd[j++] = '1';
	while (cmd[i])
	{
		if (i == first_para)
			i++;
		else if (i == last_para)
			i++;
		new_cmd[j++] = cmd[i++];
	}
	free(cmd);
	return (new_cmd);
}

char	*check_for_parentheses(char *cmd, t_shell_stuff *sh, int *pp, int doset, int *index)
{
	int		count;
	int		start_flag;
	int		first_para;
	int		i;
	// int		index;
	// char	**cmds;

	count = 0;
	start_flag = 0;
	first_para = 0;
	// index = 0;
	i = 0;
	// cmds = ft_split_operators(loco()->piece);
	if (ft_strchr(loco()->piece, '('))
	{
		*index = 0;
		loco()->parentheses = 1;
		if (loco()->and == 1)
		{
			if (loco()->piece[0] != '0' && loco()->piece[0] != '1')
				sh->lststatus = execution_proccess_and_bonus(pp, sh, doset);
			while (cmd[i])
			{
				if (cmd[i] == '(')
				{
					if (start_flag == 0)
						first_para = i;
					start_flag = 1;
					count++;
				}
				else if (cmd[i] == ')')
					count--;
				if (count == 0 && start_flag == 1)
					break ;
				i++;
			}
			//! small check to figure out if i have a pipe at the end of my () if so i will need to output to the pipe
			int	z = i - 1;
			while (cmd[++z])
			{
				if ((cmd[z] == '&' && cmd[z + 1] == '&') || (cmd[z] == '|' && cmd[z + 1] == '|'))
					break ;
				else if (cmd[z] == '|' && cmd[z + 1] != '|')
					loco()->out_to_pipe = 1;
				if (loco()->out_to_pipe == 1)
					break ;
			}
			//! you need to create a check where if the number of ( and ) is not equal then you need to return an error
			free(loco()->piece);
			loco()->piece = ft_strdup_len(cmd, i); //* printf("new loco()->piece: %s\n", loco()->piece);
			cmd = cmd_parentheses_and_cleaner(cmd, first_para, i, sh);
		}
		else if (loco()->or == 1)
		{
			if (loco()->piece[0] != '0' && loco()->piece[0] != '1')
				sh->lststatus = execution_proccess_or_bonus(pp, sh, doset);
			while (cmd[i])
			{
				if (cmd[i] == '(')
				{
					if (start_flag == 0)
						first_para = i;
					start_flag = 1;
					count++;
				}
				else if (cmd[i] == ')')
					count--;
				if (count == 0 && start_flag == 1)
					break ;
				i++;
			}
			//! small check to figure out if i have a pipe at the end of my () if so i will need to output to the pipe
			int	z = i - 1;
			while (cmd[++z])
			{
				if ((cmd[z] == '&' && cmd[z + 1] == '&') || (cmd[z] == '|' && cmd[z + 1] == '|'))
					break ;
				else if (cmd[z] == '|' && cmd[z + 1] != '|')
					loco()->out_to_pipe = 1;
				if (loco()->out_to_pipe == 1)
					break ;
			}
			//! you need to create a check where if the number of ( and ) is not equal then you need to return an error
			free(loco()->piece);
			loco()->piece = ft_strdup_len(cmd, i); //* printf("new loco()->piece: %s\n", loco()->piece);
			cmd = cmd_parentheses_or_cleaner(cmd, first_para, i, sh);
		}
		loco()->piece = ft_split_bonus(cmd, index);
		if (ft_strchr(loco()->piece, '('))
		{
			loco()->and = 0;
			loco()->or = 0;
			check_for_operator(loco()->piece);
			check_for_parentheses(cmd, sh, pp, doset, index);
		}
		else
		{
			loco()->g_and = loco()->and;
			loco()->g_or = loco()->or;
			loco()->and = 0;
			loco()->or = 0;
			check_for_operator(loco()->piece);
			// loco()->parentheses = 0;
			return (cmd);
		}
	}
	else
		return (cmd);
}

//! -------------------------------------------------------------------------- */

//* ---------------------------------- nizz ---------------------------------- */
/*int	shft_fr_to(char *cmd, t_shell_stuff *sh, int doset)
{
	char	*tmp;
	int		index;
	int		pp[2];
	int		i;

	i = -1;
	index = 0;
	tmp = (char *)ft_calloc((ft_strlen(cmd) + 1), sizeof(char));
	if (!tmp)
		return (0);
	while (cmd[++i])
		tmp[i] = cmd[i];
	if (check_for_bonus(tmp) == 1)
	{
		while (loco()->n-- > 0)
		{
			//*	split the first two commands
			loco()->piece = ft_split_bonus(tmp, &index);
			//*	check for the type
			check_for_operator(loco()->piece);
			// check_for_wildcard(loco()->piece, doset, &pp[0]);
			//*	check for parentheses
			tmp = check_for_parentheses(tmp, sh, &pp[0], doset, &index);
			//! if ((loco()->n == 0 && doset == 1) || loco()->out_to_pipe == 1) //! might not need it
			//! 	flag_pipe = 1;
			//*	send it to the correct executer
			if (loco()->and)
				sh->lststatus = execution_proccess_and_bonus(&pp[0], sh, doset);
			else if (loco()->or)
				sh->lststatus = execution_proccess_or_bonus(&pp[0], sh, doset);
			//* replace the command with the correct exit-status
			tmp = cmd_cleaner(tmp, index, sh);
			//? dont reset the and and or in order to know after the pipe what did you have before and how
			//? you handle the command.
			//!	might not be needed after all
			if (loco()->parentheses != 1 && (loco()->g_and == 1 || loco()->g_or == 1))
			{
				loco()->g_and = 0;
				loco()->g_or = 0;
			}
			else if (loco()->parentheses != 1)
			{
				loco()->g_and = loco()->and;
				loco()->g_or = loco()->or;
			}
			loco()->and = 0;
			loco()->or = 0;
			free(loco()->piece);
			loco()->out_to_pipe = 0;
		}
	}
	else
	{
		//*	here you will need to clean the command from ( and )
		if (ft_strchr(tmp, ')'))
			tmp = clean_cmd(tmp);
		if (loco()->g_or == 1 && sh->lststatus == 0)
			return (sh->lststatus);
		tmp = check_for_wildcard_normal(tmp); //! need to create another function for if its one command with no && || 
		if (sh->doexit != -1 || shft_redirections(&tmp, sh, &doset))
		{
			pipe(pp);
			close(pp[1]);
			dup2(pp[0], STDIN_FILENO);
			return (1, sh->lststatus = 1);
		}
		if (shft_is_builtin(tmp) == 0)
			sh->lststatus = builtin_cmds(tmp, sh, doset);
		else
			sh->lststatus = command(tmp, sh, doset);
		if (sh->lststatus == -1)
		{
			non_executable_handler(tmp, sh);
			pipe(pp);
			close(pp[1]);
			dup2(pp[0], STDIN_FILENO);
		}
	}
	loco()->n = 0;
	return (sh->lststatus);
}*/
//! ----------------------------------- end ---------------------------------- */


int	shft_fr_to(char *cmd, t_shell_stuff *sh, int doset)
{
	int	pp[2];

	if (sh->doexit != -1 || shft_redirections(&cmd, sh, &doset))
	{
		pipe(pp);
		close(pp[1]);
		dup2(pp[0], STDIN_FILENO);
		return (1, sh->lststatus = 1);
	}
	if (shft_is_builtin(cmd) == 0)
		sh->lststatus = builtin_cmds(cmd, sh, doset);
	else
		sh->lststatus = command(cmd, sh, doset);
	if (sh->lststatus == -1)
	{
		non_executable_handler(cmd, sh);
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
	dup2(sh->tempfds[1], STDOUT_FILENO);
	loco()->g_and = 0;
	loco()->g_or = 0;
	return (sh->lststatus);
}
