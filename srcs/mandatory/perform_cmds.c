/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perform_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 00:30:30 by ahirzall          #+#    #+#             */
/*   Updated: 2025/03/16 03:01:26 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex.h"

int	execute_a_cmd(t_pipex *pipex, char **cmd_args, int i)
{
	char	*complete_cmd;
	int		valid_status;

	complete_cmd = NULL;
	if (processing_cmd_args(pipex, cmd_args[i], &complete_cmd, 0) == OK)
	{
		if (access(complete_cmd, X_OK) == 0)
			execve(complete_cmd, cmd_args, pipex->main_pars.raw_envp);
	}
	valid_status = is_valid_cmd_checker(pipex, complete_cmd);
	if (complete_cmd != NULL)
		free(complete_cmd);
	error_msg_termination(pipex, "Invalid Command.\n", FD2,
		valid_status);
	return (valid_status);
}

int	handle_input_cmd(t_pipex *pipex, int i)
{
	if (pipex->files.infile_fd == -1)
	{
		error_msg("CMD not working; infile error.\n", FD2);
		return (ERR_OUTFILE);
	}
	pipex->childs_pids[i] = fork();
	if (pipex->childs_pids[i] == -1)
	{
		pipex->errno_rtrn = errno;
		error_msg_termination(pipex,
			"ERROR: forking for infile cmd failed :(.\n", PERROR, errno);
	}
	if (pipex->childs_pids[i] == 0)
	{
		pipex->errno_rtrn = perform_both_io_dup2(pipex, pipex->files.infile_fd,
				pipex->pipes_fds[0][1]);
		close_all_files(pipex);
		close_all_pipes(pipex);
		execute_a_cmd(pipex, pipex->cmds_args[0], 0);
	}
	return (is_valid_cmd_checker(pipex, pipex->cmds_args[0][0]));
}

int	handle_here_doc(t_pipex *pipex, int i)
{
	pipex->childs_pids[i] = fork();
	if (pipex->childs_pids[i] == -1)
	{
		error_msg("Failed in forking here_doc child.\n", PERROR);
		return (errno);
	}
	if (pipex->childs_pids[i] == 0)
	{
		pipex->errno_rtrn = perform_both_io_dup2(pipex, STDIN_FILENO,
				pipex->pipes_fds[i][1]);
		input_here_doc(pipex);
		cleanup_pipex(pipex);
		exit(OK);
	}
	return (OK);
}

int	perform_middle_pipes_cmds(t_pipex *pipex, int i)
{
	while (i < pipex->num_cmds_args - 1)
	{
		pipex->childs_pids[i] = fork();
		if (pipex->childs_pids[i] == -1)
			error_msg_termination(pipex, "Failed in forking middle child.\n",
				PERROR, errno);
		if (pipex->childs_pids[i] == 0)
		{
			pipex->errno_rtrn = perform_both_io_dup2(pipex, pipex->pipes_fds[i
					- 1][0], pipex->pipes_fds[i][1]);
			close_all_pipes(pipex);
			close_all_files(pipex);
			execute_a_cmd(pipex, pipex->cmds_args[i], 0);
		}
		i++;
	}
	return (OK);
}

int	perform_last_cmd(t_pipex *pipex, int i)
{
	i = pipex->num_cmds_args - 1;
	pipex->childs_pids[i] = fork();
	if (pipex->childs_pids[i] == -1)
		error_msg_termination(pipex, "Failed in forking last child.\n", PERROR,
			errno);
	if (pipex->childs_pids[i] == 0)
	{
		if (pipex->files.outfile_fd == -1)
			error_msg_termination(pipex, "Failed in last cmd.", PERROR,
				get_outfile_ready(pipex));
		pipex->errno_rtrn = perform_both_io_dup2(pipex, pipex->pipes_fds[i
				- 1][0], pipex->files.outfile_fd);
		close_all_pipes(pipex);
		if (pipex->files.outfile_fd >= 0)
			close(pipex->files.outfile_fd);
		execute_a_cmd(pipex, pipex->cmds_args[i], 0);
	}
	return (OK);
}
