/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpollini <lpollini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 18:06:24 by lpollini          #+#    #+#             */
/*   Updated: 2023/07/31 14:34:49 by lpollini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdarg.h>
# include <fcntl.h>
# include <signal.h>
# include <dirent.h>
# include <curses.h>
# include <term.h>
# include <signal.h>
# include <dirent.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/ioctl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include "get_next_line.h"
# include "libft.h"

# define FILENAMELEN 4
# define BIG 999
# define ENVN 999

# define RED "\001\e[0;31m\002"
# define GREEN "\001\e[0;32m\002"
# define GREENBOLD "\001\e[1;32m\002"
# define YELLOW "\001\e[0;33m\002"
# define BLUE "\001\e[0;34m\002"
# define BLUEBOLD "\001\e[1;34m\002"
# define MAGENTA "\001\e[0;35m\002"
# define CYAN "\001\e[0;36m\002"
# define UNSET "\001\e[0m\002"
# define BOLD "\001\e[1m\002"
# define FILENAME ".file"

# define BLTINS 0

# define PROMPT "minishell_by_lpollini"
# define PROMPTSIZEMAX 199

typedef struct s_vector2_int
{
	int	a;
	int	b;
}	t_vector2_int;

typedef struct s_var_lst
{
	struct s_var_lst	*next;
	char				*value;
	char				*name;
}	t_var_lst;

typedef struct s_shell_stuff
{
	char			**cmd;
	char			*pwd;
	char			*envp[ENVN];
	int				envn;
	char			**args;
	int				argn;
	char			*usr;
	int				lststatus;
	char			haspipes;
	char			beginsquote;
	int				doexit;
	unsigned char	exit_code;
	int				tempfds[2];
}	t_shell_stuff;

typedef struct s_fds
{
	int	a_touse[2];
}	t_fds;

typedef struct s_command
{
	char	*cmd;
	char	**dquotesplit;
	char	**spacesplit;
	char	**pipelinesplit;
}	t_command;

int		ft_strcmp_noend(const char *s1, const char *s2);
int		shft_exit(int e, t_shell_stuff *sh);
char	*ft_strjoin_free(char *s1, const char *s2);
char	*ft_strjoin_free_1(char const *s1, char *s2);
int		shft_rmdirone(char *s, int lim);
char	*build_put(t_shell_stuff *sh);
char	*better_pwd(char *pwd, const char *usr);
char	*shft_prompt(t_shell_stuff *sh, int dofree);
int		shft_execute_cmd(t_shell_stuff *sh, char *str);
char	*shft_strchr(const char *s, char c, char ig1, char ig2);
char	*shft_strrchr(const char *s, char c, char ig1, char ig2);
char	**ft_split1(char const *s, char c, char iga);
char	**shft_split(char const *s, char c, char ig1, char ig2);
char	**shft_split1(char *s, char c, char ig1, char ig2);
int		shft_pipexexec(char **cmds, int pipes, t_shell_stuff *sh);
int		shft_strcmp_noend(const char *s1, const char *s2);
int		shft_strcmp_noend2(const char *s1, const char *s2);
int		shft_isalnum(const int c);
int		shft_atoi(const char *str);
char	*shft_getenv(char *find, char *envp[], t_shell_stuff *sh);
char	*shft_env_dup(char *in[], int i, int *r);
int		shft_getenv_line(char *find, char *envp[], t_shell_stuff *sh);
void	update_env_free(char **envp, char *pwd, t_shell_stuff *sh);
int		shft_env_init_free(char *env[], char *in[], int sw);
char	*ft_strjoin_by(char const *s1, char const *s2, char sep);
char	*good_strdup(const char *src);
int		shft_istab(char c);
int		shft_split1_test(char *s, char ig1, char ig2, int test);
int		shft_rmdirone(char *s, int lim);
int		shft_pwd_better_1(char *res, char *nw, char **ori);
void	shft_init_two_vars(int *a, int va, int *b, int vb);
int		count_words(const char *str, char c);
char	*word_dup(char const *str, int start, int finish);

int		shft_cmd_pwd(char *cmd, t_shell_stuff *sh);
int		shft_cmd_env(char *cmd, t_shell_stuff *sh);
int		shft_cmd_echo(char *cmd, t_shell_stuff *sh);
int		shft_cmd_exit(char *cmd, t_shell_stuff *sh);
int		shft_cmd_cd(char *cmd, t_shell_stuff *sh);
int		shft_cmd_unset(char *cmd, t_shell_stuff *sh);
int		shft_cmd_export(char *cmd, t_shell_stuff *sh);

#endif
