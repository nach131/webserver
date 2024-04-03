# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vduchi <vduchi@student.42barcelona.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/28 10:03:28 by vduchi            #+#    #+#              #
#    Updated: 2024/03/12 15:32:04 by vduchi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#=-=-=-=-=-=-=- COLORS DEFINITION =-=-=-=-=-=-=-=-=-#

DEL_LINE		=	\033[2K
ITALIC			=	\033[3m
BOLD			=	\033[1m
DEF_COLOR		=	\033[0;39m
GRAY			=	\033[0;90m
RED				=	\033[0;91m
GREEN			=	\033[0;92m
YELLOW			=	\033[0;93m
BLUE			=	\033[0;94m
MAGENTA			=	\033[0;95m
CYAN			=	\033[0;96m
WHITE			=	\033[0;97m
BLACK			=	\033[0;99m
ORANGE			=	\033[38;5;209m
BROWN			=	\033[38;2;184;143;29m
DARK_GRAY		=	\033[38;5;234m
MID_GRAY		=	\033[38;5;245m
DARK_GREEN		=	\033[38;2;75;179;82
DARK_YELLOW		=	\033[38;5;143m

NAME		=	webserv

SRCS_DIR	=	src
INCS_DIR	=	inc
OBJS_DIR	=	.objs
DEPS_DIR	=	.deps

SRCS		=	src/main.cpp
OBJS		=	$(patsubst $(SRCS_DIR)/%, $(OBJS_DIR)/%, $(SRCS:.cpp=.o))
DEPS		=	$(patsubst $(SRCS_DIR)/%, $(OBJS_DIR)/%, $(SRCS:.cpp=.d))

CPPFLAGS	+=	-Wall -Werror -Wextra -std=c++98 -pedantic-errors $(addprefix -I , $(INCS_DIR))
CPPFLAGS	+=	-MMD -MP -MF $(DEPS_DIR)/$*.d

CXX			=	c++
RM			=	rm -rf
MKDIR		=	mkdir -p
MAKE		=	make --no-print-directory

$(OBJS_DIR)/%.o		:	$(SRCS_DIR)/%.cpp
	@$(CXX) $(CPPFLAGS) -c $< -o $@
	@echo "$(YELLOW)$(patsubst $(SRCS_DIR)/%,%, $<)	\tcompiled!$(DEF_COLOR)"

all		:	directories $(NAME)

$(NAME)	:	$(OBJS)
	@$(CXX) $^ -o $@
	@echo "$(GREEN)Executable $@ compiled!$(DEF_COLOR)"

directories:
	@$(MKDIR)	$(OBJS_DIR)
	@$(MKDIR)	$(DEPS_DIR)

clean	:
	@$(RM)	$(OBJS_DIR)
	@$(RM)	$(DEPS_DIR)
	@$(RM)	$(OBJS) $(DEPS)

fclean	:	clean
	@$(RM) $(NAME)
	@echo "$(BLUE)$(NAME) cleaned!$(DEF_COLOR)"

re		:	fclean all

.PHONY: all clean fclean re

-include $(DEPS)
