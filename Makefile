# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: albernar <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/16 17:32:26 by albernar          #+#    #+#              #
#    Updated: 2025/04/02 18:44:41 by albernar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# VARIABLES
CC 			:=	c++
FLAGS 		:=	-Wall -Wextra -Werror -std=c++98 -g

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

SRCS		:=	$(addprefix $(SRC_DIR)/, $(SRCS))

SRCS_BONUS	:=	main.cpp
SRCS_BONUS	:=	$(addprefix $(SRC_DIR)/, $(SRCS_BONUS))

# OBJECTS
OBJS		=	$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
OBJS_BONUS	=	$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS_BONUS))

# EXECUTABLES
NAME = ircserv
NAME_BONUS = ircserv_bonus

# RULES
all: $(NAME)
bonus: $(NAME_BONUS)

$(NAME): $(OBJS)
	@$(CC) $(FLAGS) $(INCLUDES) $(OBJS) -o $(NAME)
	@echo " $(GREEN)$(BOLD)$(ITALIC)■$(RESET)  building	$(GREEN)$(BOLD)$(ITALIC)$(NAME)$(RESET)"

$(NAME_BONUS): $(OBJS_BONUS)
	@$(CC) $(FLAGS) $(INCLUDES) $(OBJS_BONUS) -o $(NAME_BONUS)
	@echo " $(GREEN)$(BOLD)$(ITALIC)■$(RESET)  building	$(GREEN)$(BOLD)$(ITALIC)$(NAME_BONUS)$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo " $(CYAN)$(BOLD)$(ITALIC)■$(RESET)  compiling	$(GRAY)$(BOLD)$(ITALIC)$<$(RESET)"
	@$(CC) $(FLAGS) $(INCLUDES) -o $@ -c $<

clean:
	@echo " $(RED)$(BOLD)$(ITALIC)■$(RESET)  cleaned	$(RED)$(BOLD)$(ITALIC)$(MLX_DIR)$(RESET)"
	@rm -rf $(OBJ_DIR)
	
fclean: clean
	@echo "$(RED)Cleaning executables...$(RESET)"
	@rm -f $(NAME) $(NAME_BONUS)
	
re: fclean all

.PHONY: all bonus clean fclean re

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