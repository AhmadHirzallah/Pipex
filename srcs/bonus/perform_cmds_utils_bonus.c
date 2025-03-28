/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perform_cmds_utils_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 00:30:26 by ahirzall          #+#    #+#             */
/*   Updated: 2025/03/16 03:07:07 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex.h"

void	input_here_doc(t_pipex *pipex)
{
	while (true)
	{
		if (process_here_doc_line(pipex) == HERE_DOC_STOP)
			break ;
	}
}

int	is_valid_cmd_checker(t_pipex *pipex, char *cmd)
{
	if (access(cmd, F_OK) == -1)
	{
		pipex->errno_rtrn = errno;
		return (ERR_CMD_NOT_EXIST);
	}
	if (access(cmd, R_OK | X_OK) == -1)
	{
		pipex->errno_rtrn = errno;
		return (ERR_CMD_NOT_XR);
	}
	return (OK);
}

static int	process_pathed_entered_cmd(t_pipex *pipex, char **cmd_args,
						char **complete_cmd, int i)
{
	if (cmd_args == NULL || cmd_args[i] == NULL)
		return ((int)PER_NOT_EXIST_F_NOK);
	else if (ft_strchr(cmd_args[i], '/') != NULL)
	{
		if (is_valid_cmd_checker(pipex, cmd_args[i]) != OK)
			error_msg_termination(pipex, "ERROR not valid command.\t", PERROR,
				pipex->errno_rtrn);
		else
		{
			*complete_cmd = cmd_args[i];
			return (OK);
		}
	}
	return (1);
}

static int	process_general_cmd(t_pipex *pipex, char **cmd_args,
									char **complete_cmd, int i)
{
	char	*temp_cmd;
	char	*temp_complete;
	int		validation_return;

	temp_cmd = join_cmds_handler(pipex, "/", cmd_args[i], false);
	i = 0;
	while (pipex->system_paths[i])
	{
		temp_complete = join_cmds_handler(pipex, pipex->system_paths[i],
				temp_cmd, false);
		validation_return = is_valid_cmd_checker(pipex, temp_complete);
		if (validation_return == OK)
		{
			*complete_cmd = temp_complete;
			free(temp_cmd);
			return (OK);
		}
		free(temp_complete);
		i++;
	}
	free(temp_cmd);
	*complete_cmd = NULL;
	error_msg_termination(pipex, "Invalid Command.\n", FD2, validation_return);
	return (validation_return);
}

int	processing_cmd_args(t_pipex *pipex, char *cmd_args,
				char **complete_cmd, int i)
{
	if (process_pathed_entered_cmd(pipex, &cmd_args, complete_cmd, i) == OK)
		return (OK);
	return (process_general_cmd(pipex, &cmd_args, complete_cmd, i));
}
