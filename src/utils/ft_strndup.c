#include <stdlib.h>

char	*ft_strndup(const char *s1, size_t n)
{
	char	*dup;
	size_t	i;

	dup = (char *)malloc(sizeof(char) * (n + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (s1[i] && i < n)
	{
		dup[i] = s1[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
