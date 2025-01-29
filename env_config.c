/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_config.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luisfederico <luisfederico@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 16:24:23 by luisfederic       #+#    #+#             */
/*   Updated: 2025/01/29 17:41:23 by luisfederic      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_array(char **array)
{
    int i;

    int i = 0;
    if(!array)
        return ;
    while(array[i])
    {
        free(array[i]);
        array[i] = NULL;
        i++;
    }
    free(array);
}

t_env *new_env_node(char *name, char *content, int env, int exp)
{
    t_env *newnode;

    newnode = (t_env *)malloc(sizeof(t_env));
    if(!newnode)
    {
        return(NULL);
    }
    if(name)
    {
        newnode->name = ft_strdup(name, 0);
    }
    else
        newnode->name = ft_strdup("", 0);
    if(content)
        newnode->content = ft_strdup(content, 0);
    else
        newnode->content = ft_strdup("", 0);
    newnode->is_env = env;
    newnode->is_exp = exp;
    newnode->is_pri = FALSE;
    newnode->next = NULL;
    return(newnode);
}

/*Agrega una variable de entorno a la listaexistente y actualiza la estructura de variables de entorno del proceso mini*/
/*Se hace para permitir que el proceso acceda a las variables de entorno definidas en el sistema*/
void set_env(char **env, t_minishell *mini)
{
    t_env *cur_node;
    t_env *newnode;
    char **divide_env;

    while(*env != NULL)
    {
        divide_env = ft_split(*env, '=');
        newnode = new_env_node(divide_env[0], divide_env[1], 1, 0);
        if(!mini -> env)
            mini->env = newnode;
        else
        {
            cur_node = mini->env;
            while(cur_node->next)
            {
                cur_node = cur_node->next;
            }
            cur_node->next = newnode;
        }
        free_array(divide_env);
        env++;
    }
}