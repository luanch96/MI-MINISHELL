# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: luisfederico <luisfederico@student.42.f    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/20 13:30:06 by luisfederic       #+#    #+#              #
#    Updated: 2025/01/29 16:21:45 by luisfederic      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


DEL			=	rm -f
CC			=	gcc
CCFLAGS		=	-Wall -Wextra -Werror -lreadline -g3 #-fsanitize=leak

#Nombre ejecutable
NAME		=	minishell

#Ficheros
SRC_FILES	=	main.c \
				
SRC			=	$(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_FILES)))
OBJ			=	$(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILES)))
LIBFT		=	libft

#Librerias
LIBS		= $(LIBFT)/libft.a

#Directorios
SRC_DIR = src/
OBJ_DIR = obj/

# REGLAS #
all: libft $(NAME)

#Compilar
$(NAME):$(OBJ)
		@$(CC) $(OBJ) $(LIBS) $(CCFLAGS) -o $(NAME)
		@echo "$(GREEN)MINISHELL HAS BEEN COMPILED!$(NC)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	@echo "$(YELLOW)Compiling: $<$(NC)"
	@$(CC) $(CCFLAGS) $(RLFLAGS) -o $@ -c $<
	@echo "$(YELLOW)Compiled!$(NC)"

#compilar librerias individuales
libft:
	@echo "$(YELLOW)COMPILING LIBFT...$(NC)"
	@make -C $(LIBFT)
	@echo "$(GREEN)LIBFT HAS BEEN COMPILED$(NC)"

# Eliminar tmp libft
fclean_libft:
	@make fclean -C $(LIBFT)
	@echo "$(RED)LIBFT FULL CLEANED!$(NC)"

# Eliminar temporales
clean:
	@$(RM) -r $(OBJ_DIR)
	@echo "$(RED)OBJS AND DIRECTORY CLEANED!$(NC)"

# Eliminar temporales y ejecutable
fclean: clean fclean_libft
	@$(RM) $(NAME)
	@echo "$(RED)EXECUTABLE CLEANED!$(NC)"

re: fclean all