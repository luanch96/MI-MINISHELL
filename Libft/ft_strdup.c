/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luisfederico <luisfederico@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 11:32:56 by luisanch          #+#    #+#             */
/*   Updated: 2025/01/29 17:19:12 by luisfederic      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include "libft.h"

char	*ft_strdup(const char *s, int mode)
{
	size_t	i;
	size_t	size;
	char	*d;

	i = 0;
	size = ft_strlen((char *)s) + 1;
	d = (char *)malloc(size);
	if (d == 0)
		return (NULL);
	while (s[i] != '\0')
	{
		d[i] = s[i];
		i++;
	}
	d[i] = '\0';
	if (mode == 1)
		free((char *)s);
	return (d);
}

/*char	*ft_strdup(const char *string1)
{
	int		x;
	int		lenght;
	char	*string2;

	lenght = ft_strlen(string1);
	string2 = (char *)malloc(sizeof(*string2) * (lenght + 1));
	if (!string2)
		return (NULL);
	x = 0;
	while (x < lenght)
	{
		string2[x] = string1[x];
		x++;
	}
	(string2[x] = '\0');
	return (string2);
}*/

/*while (s1[lenght])
		lenght++; */
/*The  strdup() function returns a pointer to a new string 
which is a duplicate
 of the string s.  Memory
for the new string is obtained with malloc(3), and can be freed with free(3).*/