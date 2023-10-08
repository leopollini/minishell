/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 13:32:51 by lpollini          #+#    #+#             */
/*   Updated: 2023/10/06 15:16:33 by naal-jen         ###   ########.fr       */
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
	init_bonus_struct();
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

//* -------------- a way to have a struct by calling a function -------------- */

t_loco	*loco(void)
{
	static t_loco	loco;

	return (&loco);
}

void	init_bonus_struct(void)
{
	loco()->and = 0;
	loco()->or = 0;
	loco()->g_and = 0;
	loco()->g_or = 0;
	loco()->parentheses = 0;
	loco()->n = 0;
	loco()->out_to_pipe = 0; //! might not be needed
	loco()->piece = NULL;
}

//! -------------------------------------------------------------------------- */

int	main(int argn, char *args[], char *envp[])
{
	t_shell_stuff	shell;
	char			*cmd_buff;

	shft_init(&shell, args, envp, argn);
	// printf("here are the experiment and: %d\n now or: %d\n now para: %d\n", loco()->and, loco()->or, loco()->parentheses);
	while (shell.doexit == -1)
	{
		cmd_buff = shft_prompt(&shell, 0);
		g_isrunning = 1;
		shell.lststatus = 0;
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