/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etien <etien@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 13:38:21 by etien             #+#    #+#             */
/*   Updated: 2024/07/01 13:36:56 by etien            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// an error returned from read might suggest that the file is not readable
// (see read(fd, NULL, 0) < 0)
// stash is freed at the start because that is the only data that persists
// across function calls
char *get_next_line(int fd)
{
	static char	*stash;
	char		*buffer;
	char		*complete_line;
	char		*tmp_stash;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	if (!stash)
		stash = ft_strdup("");
	while (!(complete_line_found(stash)))
	{
		buffer = read_to_buffer(fd);
		if (!buffer)
			return (NULL);
		tmp_stash = ft_strdup(stash);
		free(stash);
		stash = ft_strjoin(tmp_stash, buffer);
		free(tmp_stash);
		free(buffer);
	}
	complete_line = extract_line(&stash);
	return (complete_line);
}
