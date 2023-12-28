#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (dstsize == 0)
		return (strlen(src));
	while (i < dstsize - 1 && *(src + i) != 0)
	{
		*(dst + i) = *(src + i);
		i++;
	}
	if (*(dst + i))
		*(dst + i) = '\0';
	return (strlen(src));
}

static int	count_words(char const *s, char c)
{
	int	i;
	int	words;
	int	flag;

	i = 0;
	words = 0;
	flag = 1;
	while (*(s + i) != '\0')
	{
		if (*(s + i) != c && flag == 1)
		{
			words++;
			flag = 0;
		}
		if (*(s + i) == c)
			flag = 1;
		i++;
	}
	return (words);
}

static int	count_len(const char *s, char c)
{
	int	i;

	i = 0;
	while (*(s + i) != '\0' && *(s + i) != c)
		i++;
	return (i);
}

static int	find_start(const char *s, char c)
{
	int	i;

	i = 0;
	while (*(s + i) == c)
		i++;
	return (i);
}

static int	find_next_start(const char *s, char c)
{
	int	i;

	i = 0;
	while (*(s + i) != c && *(s + i) != '\0')
		i++;
	i += find_start(&s[i], c);
	return (i);
}

char	**ft_split(char *s, char c)
{
	int		i;
	int		start;
	int		words;
	char	**ret;

	if (!s)
		return (NULL);
	words = count_words(s, c);
	ret = malloc(sizeof(char *) * (words + 1));
	if (!ret)
		return (NULL);
	i = 0;
	start = find_start(s, c);
	while (i < words)
	{
		ret[i] = malloc(sizeof(char) * count_len(&s[start], c) + 1);
		if (!ret[i])
			return (NULL);
		ft_strlcpy(ret[i], &s[start], count_len(&s[start], c) + 1);
		start += find_next_start(&s[start], c);
		i++;
	}
	ret[i] = NULL;
	return (ret);
}
