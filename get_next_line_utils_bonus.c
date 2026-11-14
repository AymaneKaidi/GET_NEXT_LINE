/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaidi <akaidi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 21:19:08 by akaidi            #+#    #+#             */
/*   Updated: 2026/11/12 21:19:11 by akaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strchr_nl(const char *s)
{
	if (!s)
		return (NULL);
	while (*s)
	{
		if (*s == '\n')
			return ((char *)s);
		s++;
	}
	return (NULL);
}

char	*ft_strjoin_free(char *stash, char *buf)
{
	char	*p;
	size_t	i;
	size_t	j;

	p = malloc(ft_strlen(stash) + ft_strlen(buf) + 1);
	if (!p)
	{
		free(stash);
		return (NULL);
	}
	i = 0;
	while (stash && stash[i])
	{
		p[i] = stash[i];
		i++;
	}
	j = 0;
	while (buf && buf[j])
		p[i++] = buf[j++];
	p[i] = '\0';
	free(stash);
	return (p);
}

char	*ft_copy(const char *src, size_t len)
{
	char	*p;
	size_t	i;

	p = malloc(len + 1);
	if (!p)
		return (NULL);
	i = 0;
	while (i < len && src[i])
	{
		p[i] = src[i];
		i++;
	}
	p[i] = '\0';
	return (p);
}
