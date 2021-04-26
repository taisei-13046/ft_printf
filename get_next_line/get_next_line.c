#include "get_next_line.h"
#include <stdio.h>

//#define BUFFER_SIZE 65

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

static char	*sub_strcpy(char *str_a, char *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		str_a[i] = src[i];
		i++;
	}
	str_a[i] = '\0';
	return (str_a);
}

char	*ft_strdup(const char *str)
{
	char	*str_a;
	char	*src;

	src = (char *)str;
	str_a = (char *)malloc((sizeof(char) * ft_strlen(src) + 1));
	if (!str_a)
		return (NULL);
	str_a = sub_strcpy(str_a, src);
	return (str_a);
}

char	*ft_strchr(const char *str, int c)
{
	c = (char)c;
	if (!str)
		return (NULL);
	while (*str != c)
	{
		if (*str == '\0')
			return (NULL);
		str++;
	}
	return ((char *)str);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*p;
	int		i;

	p = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!p)
		return (NULL);
	i = 0;
	while (s1 != NULL && *s1)
	{
		p[i] = *s1;
		i++;
		s1++;
	}
	while (*s2)
	{
		p[i] = *s2;
		i++;
		s2++;
	}
	p[i] = '\0';
	return (p);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char *p;
	int i;

	i = 0;
	s = (char *)s;
	if (!(p = malloc(sizeof(char *) * len + 1)))
		return (NULL);
	while(len-- > 0)
	{
		p[i] = s[start];
		i++;
		start++;
	}
	p[i] = 0;
	return (p);
}

//void	save_free(char **save, size_t len)
//{
//	int	i;

//	i = 0;
//	while (i < len)
//	{
//		free(save[i]);
//		i++;
//	}
//}

char	*split_save_after(char *save)
{
	size_t	len;
	char	*tmp;

	len = 0;
	while (save[len] != '\n' && save[len])
		len++;
	tmp = ft_substr(&save[len], 1, ft_strlen(save));
	//どういう時にここでfreeするのか？
	free(save);
	save = NULL;
	return (tmp);
}

char	*split_save(char *save, int *flag)
{
	char	*tmp;
	size_t	len;

	len = 0;
	tmp = (char *)malloc(sizeof(char *) * (ft_strlen(save) + 1));
	while (save[len] != '\n' && save[len])
	{
		tmp[len] = save[len];
		len++;
	}
	if (ft_strchr(&save[len], '\n') || !save)
		(*flag) = 1;
	return (tmp);
}

int	get_next_line(int fd, char **line)
{
	int				flag;
	ssize_t			rd_cnt;
	char			*buf;
	static char		*save;
	char			*tmp;

	flag = 0;
	if (fd < 0 || !line || BUFFER_SIZE < 0)
		return (-1);
	*line = malloc(1);
	*line[0] = 0;
	if (ft_strchr(save, '\n'))
	{
		tmp = split_save(save, &flag);
		save = split_save_after(save);
	}
	buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!ft_strchr(save, '\n') && flag == 0)
		while (flag == 0 && (rd_cnt = read(fd, buf, BUFFER_SIZE)) > 0)
		{
			if (rd_cnt == 0)
			{
				flag = 0;
				break ;
			}
			buf[rd_cnt] = 0;
			if (ft_strchr(buf, '\n'))
				flag = 1;
			save = ft_strjoin(save, buf);
			tmp = split_save(save, &flag);
			save = split_save_after(save);
			if ((*line) != 0)
			{
				if (rd_cnt != 1)
				{
					free(*line);
					*line = NULL;
				}
				*line = ft_strjoin(*line, tmp);
			}
			else
			{
				free(*line);
				*line = NULL;
				*line = tmp;
			}
			if (**line == 0)
				*line = ft_strdup("");
		}
	else
	{
		free(*line);
		*line = NULL;
		*line = tmp;
	}
	//if (flag == 0)
	//	free(save);
	free(buf);
	return (flag);
}

//#include <stdio.h>
//int	main(void)
//{
//	char	*line;
//	int		fd;
//	int flag;

//	fd = open("test.txt", O_RDONLY);
//	if (fd == -1)
//		return (0);
//	while ((flag = get_next_line(fd, &line)) > 0)
//	{
//		printf("%d: ", flag);
//		printf("%s\n", line);
//	}
//	printf("%d: ", flag);
//	printf("%s", line);
//	system("leaks a.out");
//	return (0);
//}
