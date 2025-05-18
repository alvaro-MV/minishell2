/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvez-dia <lvez-dia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 18:01:07 by lvez-dia          #+#    #+#             */
/*   Updated: 2025/05/17 15:46:10 by lvez-dia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "builtins/builtins.h"
# include "data_structs/dictionary.h"
# include "data_structs/dynamic_array.h"
# include "execution/execution.h"
# include "expansion/expansion.h"
# include "libft/include/libft.h"
# include "parser/parser.h"
# include "signals.h"
# include "tokenizer/tokenizer.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>

char	**dict_envp(t_dictionary *env);
void	dict_set_env_var(t_dictionary **env, char *k, char *v, int export);
void	insert_special_params(t_dictionary **env);
void	process_env_variables(t_dictionary **hash_env, char **env);
void	get_env(t_dictionary **hash_env, char **env);
void	insert_status(int status, t_dictionary **dict);
void	init_environment(t_dictionary **hash_env, char **env, char **line,
			int *finish);
void	process_commands(t_dictionary *hash_env, char *line);
char	*expand_str(char *str, t_dictionary *env);

#endif