/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etien <etien@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 13:38:44 by etien             #+#    #+#             */
/*   Updated: 2024/07/01 13:16:11 by etien            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// Returns length of string.
size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

// Copies memory.
void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;

	if (!dst && !src)
		return (dst);
	d = (unsigned char *)dst;
	s = (const unsigned char *)src;
	while (n--)
	{
		*d++ = *s++;
	}
	return (dst);
}

// Duplicates string.
char	*ft_strdup(const char *s1)
{
	char	*dst;
	size_t	len;

	len = ft_strlen(s1) + 1;
	dst = (char *)malloc(len);
	if (!dst)
		return (NULL);
	ft_memcpy(dst, s1, len);
	return (dst);
}

// Extracts substring from string.
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*substr;

	if (!s)
		return (NULL);
	if (start > ft_strlen(s) - 1)
		return (ft_strdup(""));
	if (start + len > ft_strlen(s))
		len = ft_strlen(s) - start;
	substr = malloc((len + 1) * sizeof(char));
	if (!substr)
		return (NULL);
	i = 0;
	while (i < len)
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}

// Joins two strings.
char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (NULL);
	str = (char *)malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i])
		str[j++] = s1[i++];
	i = 0;
	while (s2[i])
		str[j++] = s2[i++];
	str[j] = '\0';
	return (str);
}

// read function returns ssize_t, a signed type that can hold both positive values
// (indicating the number of bytes read) and negative values
// (indicating an error, with -1 being the most common error return value).
// storing the return value of read in an int is standard practice
// This function calls the read function which will store the results within a buffer.
// The buffer is null-terminated to simplify string manipulation later on and returned.
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
	return buffer;
}

// Runs through existing stash checking for newline character.
// Returns 1 if a newline character is found to terminate the while loop.
// Otherwise new buffers will continue to be appended to stash.
int complete_line_found(char *stash)
{
	while (*stash != '\n' && *stash)
		stash++;
	if (*stash == '\n')
		return (1);
	return (0);
}

// **stash is used instead of *stash because we want to move the stash pointer to the
// leftovers string memory after freeing the original stash.
// This function checks within the stash for a complete line based on the presence
// of a newline character. If one is found, the stash is substringed into the
// complete line (which is returned) and a leftover string.
// The stash pointer will be updated to point to the leftover string.
// On the other hand, if no newline character is found, it means the line is still incomplete
// and the function will just return a NULL.
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
