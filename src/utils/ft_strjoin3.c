#include <stdlib.h>
#include "libft.h"  // Assuming this contains declarations for ft_strlen and ft_strlcpy
#include "minishell.h"

char	*ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
	char	*result;
	size_t	len1;
	size_t	len2;
	size_t	len3;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len3 = ft_strlen(s3);
	result = (char *)malloc(sizeof(char) * (len1 + len2 + len3 + 1));
	if (!result)
		return (NULL);
	ft_strlcpy(result, s1, len1 + 1);
	ft_strlcat(result, s2, len1 + len2 + 1);
	ft_strlcat(result, s3, len1 + len2 + len3 + 1);
	return (result);
}