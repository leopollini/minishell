/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpollini <lpollini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 13:32:51 by lpollini          #+#    #+#             */
/*   Updated: 2023/08/18 15:16:42 by lpollini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	g_isrunning;

void	sigint_handle(int a)	//please handle sigintyy
{
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	if (!g_isrunning)
	rl_redisplay();
}

void	shft_init(t_shell_stuff *sh, char *args[], char *envp[], int argn)
{
	sh->tempfds[1] = dup(STDOUT_FILENO);
	sh->tempfds[0] = dup(STDIN_FILENO);
	signal(SIGINT, &sigint_handle);
	signal(SIGQUIT, &sigint_handle);
	sh->lststatus = 0;
	sh->usr = getenv("USER");
	if (!sh->usr)
		sh->usr = ft_calloc(1, 8);
	sh->envn = shft_env_init_free(sh->envp, envp, 0);
	sh->args = args;
	sh->argn = argn;
	sh->pwd = ft_strdup(getenv("PWD"));
	if (!sh->pwd || access(sh->pwd, F_OK) == -1)
		sh->pwd = ft_strdup("/");
	update_env_free(sh->envp, sh->pwd, sh);
	sh->doexit = -1;
	sh->exit_code = 0;
	g_isrunning = 0;
}

int	shft_exit(int e, t_shell_stuff *sh)
{
	shft_prompt(sh, 1);
	sh->envn = shft_env_init_free(sh->envp, NULL, 1);
	shft_cmd_cd(NULL, sh);
	free(sh->pwd);
	rl_clear_history();
	exit(sh->exit_code);
}

int	main(int argn, char *args[], char *envp[])
{
	t_shell_stuff	shell;
	char			*cmd_buff;

	shft_init(&shell, args, envp, argn);
	while (shell.doexit == -1)
	{
		cmd_buff = shft_prompt(&shell, 0);
		g_isrunning = 1;
		if (cmd_buff && *cmd_buff)
			add_history(cmd_buff);
		if (cmd_buff)
		{
			if (*cmd_buff)
				shft_execute_cmd(&shell, cmd_buff);
			free(cmd_buff);
		}
		else
		{
			write(1, "exit\n", 5);
			break ;
		}
		update_env_free(shell.envp, shell.pwd, &shell);
		g_isrunning = 0;
	}
	shft_exit(-1, &shell);
}
