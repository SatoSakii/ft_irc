# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: albernar <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/16 17:32:26 by albernar          #+#    #+#              #
#    Updated: 2025/04/08 00:20:30 by albernar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# VARIABLES
CC 			:=	c++
FLAGS 		:=	-Wall -Wextra -Werror -std=c++98 -g -DBONUS

# INCLUDES
INCLUDES	:=	-Iincludes

# FOLDERS
OBJ_DIR		=	.objs
SRC_DIR		=	srcs

# SOURCES
SRCS		:=	Channel.cpp \
				Client.cpp \
				CommandHandler.cpp \
				IRCUtils.cpp \
				main.cpp \
				Server.cpp \
				commands/Authentication.cpp \
				commands/Nick.cpp \
				commands/Quit.cpp \
				commands/Join.cpp \
				commands/Invite.cpp \
				commands/Part.cpp \
				commands/Topic.cpp \
				commands/Kick.cpp \
				commands/Privmsg.cpp \
				commands/Mode.cpp \
				bonus/Bot.cpp

SRCS		:=	$(addprefix $(SRC_DIR)/, $(SRCS))

# OBJECTS
OBJS		=	$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# EXECUTABLES
NAME = ircserv

# RULES
all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(FLAGS) $(INCLUDES) $(OBJS) -o $(NAME)
	@echo " $(GREEN)$(BOLD)$(ITALIC)■$(RESET)  building	$(GREEN)$(BOLD)$(ITALIC)$(NAME)$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo " $(CYAN)$(BOLD)$(ITALIC)■$(RESET)  compiling	$(GRAY)$(BOLD)$(ITALIC)$<$(RESET)"
	@$(CC) $(FLAGS) $(INCLUDES) -o $@ -c $<

clean:
	@echo " $(RED)$(BOLD)$(ITALIC)■$(RESET)  cleaned	$(RED)$(BOLD)$(ITALIC)$(MLX_DIR)$(RESET)"
	@rm -rf $(OBJ_DIR)
	
fclean: clean
	@echo "$(RED)Cleaning executables...$(RESET)"
	@rm -f $(NAME)
	
re: fclean all

.PHONY: all clean fclean re

BLACK		=	\033[30m
RED			=	\033[31m
GREEN		=	\033[32m
YELLOW		=	\033[33m
BLUE		=	\033[34m
MAGENTA		=	\033[35m
CYAN		=	\033[36m
WHITE		=	\033[37m
GRAY		=	\033[90m

BOLD		=	\033[1m
ITALIC		=	\033[3m

RESET		=	\033[0m
LINE_CLR	=	\33[2K\r