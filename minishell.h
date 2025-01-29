/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luisfederico <luisfederico@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:31:41 by luisfederic       #+#    #+#             */
/*   Updated: 2025/01/29 17:55:11 by luisfederic      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H

# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <signal.h>

# define FALSE 0
# define TRUE 1
# define ERROR -1

# define READ 0
# define WRITE 1
/*Agrega una variable de entorno a la listaexistente y actualiza la estructura de variables de entorno del proceso mini*/
/*Se hace para permitir que el proceso acceda a las variables de entorno definidas en el sistema*/
typedef struct s_minishell
{
	t_env		*env;
	char **argenv;
	char **bin_path;
	int signal;

}					t_minishell;


typedef struct s_env
{
	char *name;
	char *content;
	int	is_env;
	int is_exp;
	int is_pri;
	struct s_env *next;
	
}					t_env;

typedef struct s_node
{
	char **full_cmd;
	char*full_path;
	int in;
	int out;
	int exec;
	pid_t num_pid;
}					t_node;


int main(int argc, char **argv, char **env);