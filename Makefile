# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: naal-jen <naal-jen@student.42firenze.it    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/17 08:42:40 by lpollini          #+#    #+#              #
#    Updated: 2023/10/08 11:05:33 by naal-jen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell
	
CC			= cc -g
FLAGS		= #-Wall -Wextra -Werror
RM			= rm -rf

OBJDIR = .objFiles

FILES		= env_stuff \
executor \
ft_atoi \
ft_calloc \
ft_isalnum \
ft_itoa \
ft_memmove \
ft_putstr_fd \
ft_split_2 \
ft_split_1 \
ft_split \
ft_strchr \
ft_strdup \
ft_strlen \
ft_strncmp \
ft_strnstr \
ft_isdigit \
get_next_line \
main \
pipex_main \
prompt_stuff \
shft_cmds_echo_exit \
shft_cmds_cd_2 \
shft_cmds_export_unset \
shft_cmds_pwd \
str_stuff \
wildcard/parse \
wildcard/utils

SRC			= $(FILES:=.c)
OBJ			= $(addprefix $(OBJDIR)/, $(FILES:=.o))
HEADER		= minishell.h libft.h get_next_line.h

#Colors:
GREEN		=	\e[92;5;118m
YELLOW		=	\e[93;5;226m
GRAY		=	\e[33;2;37m
RESET		=	\e[0m
CURSIVE		=	\e[33;3m

#Debug 
ifeq ($(DEBUG), 1)
   OPTS = -g
endif

.PHONY: all clean fclean re bonus norm

all: $(NAME)

$(NAME): $(OBJ) $(HEADER)
	@$(CC) $(OBJ) -lreadline $(OPTS) -o $(NAME)
	@printf "$(_SUCCESS) $(GREEN)- Executable ready.\n$(RESET)"

$(OBJDIR)/%.o: %.c $(HEADER)
	@mkdir -p $(dir $@)
	@$(CC) $(FLAGS) $(OPTS) -c $< -o $@

bonus: all

clean:
	@$(RM) $(OBJDIR) $(OBJ)
	@printf "$(YELLOW)    - Object files removed.$(RESET)\n"

fclean: clean
	@$(RM) $(NAME)
	@printf "$(YELLOW)    - Executable removed.$(RESET)\n"

re: fclean all