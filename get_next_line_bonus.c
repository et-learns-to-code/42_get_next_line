/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etien <etien@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 13:41:10 by etien             #+#    #+#             */
/*   Updated: 2024/07/03 10:29:29 by etien            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

// (read(fd, NULL, 0) < 0) ensures that the file is in a readable format
// stash is freed at the start because the data persists across function calls
// The function does some initial checks then initializes stash.
// It calls append_buffer_loop to keep reading from the buffer and
// storing the data in stash.
// If a complete line is found, the line is extracted and returned.
// If extract_line returns null, stash is deallocated and null is returned.
// Bonus part implementation creates an fd stash array
// When fd is input into the function, the function will only operate on the
// associated fd stash
char	*get_next_line(int fd)
{
	static char	*stash[MAX_FD];
	char		*complete_line;

	if (fd >= MAX_FD || fd < 0 || BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
	{
		if (fd < MAX_FD && fd >= 0)
		{
			free(stash[fd]);
			stash[fd] = NULL;
		}
		return (NULL);
	}
	if (!stash[fd])
		stash[fd] = ft_strdup("");
	append_buffer_loop(&stash[fd], fd);
	complete_line = extract_line(&stash[fd]);
	if (!complete_line)
	{
		free(stash[fd]);
		stash[fd] = NULL;
	}
	return (complete_line);
}

// The loop will keep reading from the buffer and appending the buffer
// to the stash so long as no complete line is found.
void	append_buffer_loop(char **stash, int fd)
{
	char		*buffer;
	char		*tmp_stash;

	while (!(complete_line_found(*stash)))
	{
		buffer = read_to_buffer(fd);
		if (!buffer)
			return ;
		tmp_stash = ft_strjoin(*stash, buffer);
		free(*stash);
		free(buffer);
		*stash = tmp_stash;
	}
}

// Runs through existing stash checking for newline character.
// Returns 1 if a newline character is found to terminate the while loop.
int	complete_line_found(char *stash)
{
	while (*stash != '\n' && *stash)
		stash++;
	if (*stash == '\n')
		return (1);
	return (0);
}

// read function returns ssize_t, a signed type that can hold
// both positive values (indicating the number of bytes read) and
// negative values (indicating an error, with -1 being the most
// common error return value)
// storing the return value of read in an int is standard practice
// This function calls the read function which will store the
// results within a buffer. The buffer is null-terminated to
// simplify string manipulation later on and returned.
char	*read_to_buffer(int fd)
{
	char	*buffer;
	int		bytes_read;

	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	if (bytes_read <= 0)
	{
		free(buffer);
		return (NULL);
	}
	buffer[bytes_read] = '\0';
	return (buffer);
}

// **stash is used instead of *stash because we want to move the
// stash pointer to the leftovers string memory after freeing the
// original stash.
// This function checks within the stash for a complete line based on
// the presence of a newline character. If one is found, the stash is
// split into the substrings of complete line (which is returned) and
// a leftover string.
// The stash pointer will be updated to point to the leftover string.
// On the other hand, if no newline character is found, it means the
// line is still incomplete and the function will just return a NULL.
char	*extract_line(char **stash)
{
	size_t	len;
	char	*complete_line;
	char	*leftovers;

	len = 0;
	if (ft_strlen(*stash) == 0)
		return (NULL);
	while ((*stash)[len] != '\n' && (*stash)[len])
		len++;
	if ((*stash)[len] == '\n')
	{
		complete_line = ft_substr(*stash, 0, len + 1);
		leftovers = ft_substr(*stash, len + 1, ft_strlen(*stash) - len - 1);
		free(*stash);
		*stash = leftovers;
		return (complete_line);
	}
	else if ((*stash)[len] == '\0')
	{
		complete_line = *stash;
		*stash = NULL;
		return (complete_line);
	}
	return (NULL);
}
