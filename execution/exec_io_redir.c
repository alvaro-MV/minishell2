/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_io_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvmoral <alvmoral@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 18:19:21 by lvez-dia          #+#    #+#             */
/*   Updated: 2025/05/18 21:52:53 by alvmoral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static char *sanitize_filename(const char *raw)
{
    char    *clean;
    size_t   len;

    len = ft_strlen(raw);
    if ((raw[0] == '\'' && raw[len - 1] == '\'')
     || (raw[0] == '"'  && raw[len - 1] == '"'))
        clean = ft_substr(raw, 1, len - 2);
    else
        clean = ft_strdup(raw);
    return (clean);
}

int	override_fd(t_exec *exec, t_io_redir *redir, int flags, int idx)
{
	int			fd;
	char		*clean;
	struct stat	file_stat;

	clean = sanitize_filename(redir->filename->text);
	fd = open(clean, flags, 0644);
	if (fd == -1)
	{
		perror(clean);
		if (access(clean, F_OK))
			return (1);
		if (stat(clean, &file_stat) == -1)
			return (1);
		if (!(file_stat.st_mode & S_IWUSR) || !(file_stat.st_mode & S_IRUSR))
		{
			free_cmd(exec->cmd);
			dict_delete(exec->env);
			return (1);
		}
		else
			return (1);
	}
	else
	{
		close(exec->cmd->fds[idx]);
		exec->cmd->fds[idx] = fd;
	}
	return (0);
}

int	traverse_io_redir(t_io_redir *ix, t_exec *exec)
{
	int	status;

	status = 0;
	if (ix->op && ix->op->type != END && !ft_strncmp(ix->op->text, "<", 2))
		status = override_fd(exec, ix, O_RDONLY, 0);
	else if (ix->op && ix->op->type != END && !ft_strncmp(ix->op->text, "<<",
			2))
	{
		exec->cmd->fds[0] = ix->fd;
		status = 0;
	}
	else if (ix->op && ix->op->type != END && !ft_strncmp(ix->op->text, ">", 2))
		status = override_fd(exec, ix, O_RDWR | O_CREAT | O_TRUNC, 1);
	else if (ix->op && ix->op->type != END && !ft_strncmp(ix->op->text, ">>",
			2))
		status = override_fd(exec, ix, O_RDWR | O_APPEND | O_CREAT, 1);
	return (status);
}

int	execute_io_redir(t_exec *exec)
{
	t_io_redir	*prefix;
	t_io_redir	*suffix;
	int			status;

	prefix = exec->cmd->cmd_prefix;
	suffix = exec->cmd->cmd_suffix;
	status = 0;
	while (prefix)
	{
		status = traverse_io_redir(prefix, exec);
		if (status != 0)
			return (status);
		prefix = prefix->next;
	}
	while (suffix)
	{
		status = traverse_io_redir(suffix, exec);
		if (status != 0)
			return (status);
		suffix = suffix->next;
	}
	return (status);
}
