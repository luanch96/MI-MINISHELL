/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luisfederico <luisfederico@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:32:57 by luisfederic       #+#    #+#             */
/*   Updated: 2025/01/29 18:10:05 by luisfederic      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

static void handler_true(int signl)
{
    if(signl == SIGINT)
    {
        write(STDOUT_FILENO, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
    }
    return ;
}

static void handler_false(int signl)
{
    if(signl = SIGINT)
    {
        write(STDOUT_FILENO, "\n")
    }
}

void start_signal(int *signal_cmd)
{
    if(*signal_cmd)
    {
        signal(SIGINT, &handler_true);
        signal(SIGQUIT, &handler_false);
    }
    else if(!*signal_cmd)
    {
        signal(SIGINT, &handler_false);
        signal(SIGQUIT, SIG_IGN);
        *signal_cmd = FALSE;
    }
}

void init_minishell(t_minishell *minic+)
{
    while(1)
    {
        mini->signal = FALSE;
        init_si
        
    }
}



int main(int argc, char **argv, char **env)
{
    t_minishell mini;
    char *input;

    if(argc > 1)
        return(1);
    (void)argv;
    input = NULL;
    ft_memset(&mini, 0, sizeof(mini));
    mini.argenv = env;
    set_env(env, &mini);
    set_exit_status(&mini);
    init_minishell(&mini, input);
    free_main(&mini);
    return(0);
}
