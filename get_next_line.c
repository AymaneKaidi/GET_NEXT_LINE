/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaidi <akaidi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/08 09:32:53 by akaidi            #+#    #+#             */
/*   Updated: 2026/11/11 16:29:32 by akaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*extract_line(char *stash)
{
	char	*nl;
	size_t	len;

	nl = ft_strchr_nl(stash);
	if (nl)
		len = (size_t)(nl - stash) + 1;
	else
		len = ft_strlen(stash);
	return (ft_copy(stash, len));
}

static char	*trim_stash(char *stash)
{
	char	*nl;
	char	*rest;

	nl = ft_strchr_nl(stash);
	if (!nl || !nl[1])
	{
		free(stash);
		return (NULL);
	}
	rest = ft_copy(nl + 1, ft_strlen(nl + 1));
	free(stash);
	return (rest);
}

static char	*read_to_stash(int fd, char *stash)
{
	char	*buf;
	ssize_t	n;

	buf = malloc((size_t)BUFFER_SIZE + 1);
	if (!buf)
		return (free(stash), NULL);
	n = 1;
	while (!ft_strchr_nl(stash) && n > 0)
	{
		n = read(fd, buf, BUFFER_SIZE);
		if (n < 0)
			return (free(buf), free(stash), NULL);
		if (n == 0)
			break ;
		buf[n] = '\0';
		stash = ft_strjoin_free(stash, buf);
		if (!stash)
			return (free(buf), NULL);
	}
	free(buf);
	return (stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = read_to_stash(fd, stash);
	if (!stash)
		return (NULL);
	line = extract_line(stash);
	if (!line)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	stash = trim_stash(stash);
	return (line);
}
