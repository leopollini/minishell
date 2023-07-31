/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shft_cmds_pwd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpollini <lpollini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 20:34:40 by lpollini          #+#    #+#             */
/*   Updated: 2023/07/31 14:38:29 by lpollini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	shft_istab(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' \
		|| c == '\r' || c == '\v' || c == '\f')
		return (1);
	return (0);
}

int	shft_cmd_pwd(char *cmd, t_shell_stuff *sh)
{
	if (BLTINS)
		printf("PWD BUILTIN\n");
	printf("%s\n", shft_getenv("PWD", sh->envp, sh));
	return (0);
}
