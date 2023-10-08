/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/03 09:45:15 by lpollini          #+#    #+#             */
/*   Updated: 2023/09/27 11:48:59 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

int	ppsplit_isok(char *str)
{
	int	test;

	test = 1;
	while (*str && *str != '|')
	{
		if (*str == '\'' || *str == '\"')
			test = !test;
		str++;
	}
	if (*str == '|' && test)
	{
		str++;
		if (*str == '|')
			return (2);
		while (shft_istab(*str))
			str++;
		if (*str == '|')
			return (1);
		return (ppsplit_isok(str));
	}
	return (0);
}

int	shft_split1_test(char *s, char ig1, char ig2, int test)
{
	if (test < 0)
		test = -test;
	if (s[0] == ig1 && test != 2)
		test ^= 1;
	if (s[0] == ig2 && test != 1)
		test ^= 2;
	// if (test && s[0] == '>')
	// 	s[0] = -1;
	// if (test && s[0] == '<')
	// 	s[0] = -2;
	return (test);
}

static char	**shft_split1_1(char *s, char c, int *i, int *j)
{
	// if (ppsplit_isok(s) == 2)
	// {
	// 	// printf("hello i got here\n");
	// 	loco()->or = 1;
	// 	shft_init_two_vars(i, 0, j, 0);
	// 	return (ft_calloc(2, 8));
	// }
	/*else */if (ppsplit_isok(s) != 2 && ppsplit_isok(s))
	{
		ft_putstr_fd("Pipe: empty pipeline node\n", STDERR_FILENO);
		return (NULL);
	}
	shft_init_two_vars(i, 0, j, 0);
	while (s[*i] == ' ')
		*i = *i + 1;
	return (ft_calloc((count_words(s, c) + 1), 8));
}

char	**shft_split1(char *s, char c, char ig1, char ig2) // s is the whole command, c is the character, ig1 is ', ig2 is "
{
	t_vector2_int	v;
	int				index;
	char			**split;
	int				test;
	// printf("me s: %s\n", s);

	split = shft_split1_1(s, c, &v.a, &v.b);
	// printf("me is v.a: %d\n", v.a);
	// printf("me is v.b: %d\n", v.b);
	shft_init_two_vars(&test, 0, &index, -1);
	// printf("index: %d\ntest: %d\n", index, test);
	if (!split)
		return (NULL);
	while (v.a <= ft_strlen(s))
	{
		test = shft_split1_test((char *)s + v.a, ig1, ig2, test);
		// printf("test2: %d\n", test);
		// if (loco()->or == 1)
		// {
		// 	split[v.b++] = word_dup(s, 0, ft_strlen(s));
		// 	split[v.b] = NULL;
		// 	loco()->or = 0;
		// 	// for (int i = 0; split[i]; i++)
		// 	// 	printf("me splited: %s\n", split[i]);
		// 	return (split);
		// }
		/*else */if (s[v.a] != c && index < 0)
		{
			index = v.a;
			// printf("index2: %d\n", index);
		}
		else if (((s[v.a] == c && s[v.a + 1] != c && s[v.a - 1] != c && !test) || v.a == ft_strlen(s)) && index >= 0)
		{
			split[v.b++] = word_dup(s, index, v.a);
			index = -1;
			while (s[v.a] && s[v.a + 1] == ' ')
				v.a++;
		}
		v.a++;
	}
	split[v.b] = NULL;
	// for (int i = 0; split[i]; i++)
	// 	printf("what is: %s\n", split[i]);
	return (split);
}

char	**shft_split(char const *s, char c, char ig1, char ig2)
{
	int		i;
	int		j;
	int		index;
	char	**split;
	int		test;

	split = ft_calloc((count_words(s, c) + 1), 8);
	if (!s || !split)
		return (0);
	shft_init_two_vars(&i, 0, &j, 0);
	shft_init_two_vars(&test, 0, &index, -1);
	while (i <= ft_strlen(s))
	{
		test = shft_split1_test((char *)s + i, ig1, ig2, -test);
		if (s[i] != c && index < 0)
			index = i;
		else if (((s[i] == c && !test) || i == ft_strlen(s)) && index >= 0)
		{
			split[j++] = word_dup(s, index, i);
			index = -1;
		}
		i++;
	}
	split[j] = NULL;
	return (split);
}
