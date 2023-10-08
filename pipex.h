/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 18:45:30 by lpollini          #+#    #+#             */
/*   Updated: 2023/10/07 18:30:26 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H

# define PIPEX_H

# define WELLDONE 0
# define NEOUGHINPT 1
# define ALLRECIEVED 2
# define ALLSENT -2
# define NOINFILE -3
# define NOOUTF 3
# define CMDNOTFOUND 4
# define PIPECREATERR -4
# define OTHRERR 5
# define TRUNCFLAGS 01101
# define APPENDFLAGS 02101

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdarg.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <fcntl.h>

typedef struct s_stuff
{
	int		tempipe[2];
	int		pipefd[2];
	int		append;
	char	limiter;
}	t_stuff;

size_t	ft_strlen(const char *str);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_putstr_fd(char *str, int fd);
char	*ft_strjoin(char const *s1, char const *s2);
void	*ft_memmove(void *dest, const void *src, size_t m);
char	*ft_strdup(const char *src);
char	*get_next_line(int fd);
char	**ft_split(char const *s, char c);
void	ft_exit(int arg);
void	init(int argn, char *args[], t_stuff *stuff);
int		partloop(int *i, t_stuff *stuff, char *temp, char *args[]);
void	ft_free_tab(char **tb);

#endif
