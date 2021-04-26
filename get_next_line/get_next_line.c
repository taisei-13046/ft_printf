#include "get_next_line.h"
//#define BUFFER_SIZE 100000000

//utils


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



//get_next_line

char	*split_save_after(char *save)
{
	size_t	len;
	char	*tmp;

	len = 0;
	while (save[len] != '\n' && save[len])
		len++;
	tmp = ft_substr(&save[len], 1, ft_strlen(save));
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

void	into_line(char **line, char *tmp)
{
	if ((*line) != 0)
		*line = ft_strjoin(*line, tmp);
	else
	{
		free(*line);
		*line = NULL;
		*line = tmp;
	}
	if (**line == 0)
	{
		free(*line);
		*line = NULL;
		*line = ft_strdup("");
	}
}

char	*read_get_next_line(char *save, int fd, int *flag, char **line)
{
	char	*tmp;
	char	*buf;
	ssize_t		rd_cnt;

	buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	while (*flag == 0 && (rd_cnt = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		if (rd_cnt == 0)
		{
			*flag = 0;
			break ;
		}
		buf[rd_cnt] = 0;
		if (ft_strchr(buf, '\n'))
			*flag = 1;
		save = ft_strjoin(save, buf);
		tmp = split_save(save, flag);
		save = split_save_after(save);
		into_line(line, tmp);
	}
	if (rd_cnt == 0 && save)
		*line = save;
	free(buf);
	return (save);
}

int	get_next_line(int fd, char **line)
{
	int			flag;
	static char	*save;
	char		*tmp;

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
	if (!ft_strchr(save, '\n') && flag == 0)
		save = read_get_next_line(save, fd, &flag, line);
	else
	{
		free(*line);
		*line = NULL;
		*line = tmp;
	}
	return (flag);
}


//#include <sys/types.h>
//#include <stdio.h>

//int    main(void)
//{
//    int        d = 1;
//    char    *line;
//    int        fd;
//    int        i = 1;

//    fd = open("test.txt", O_RDONLY);
//    printf("BUFFER_SIZE: %d\n", BUFFER_SIZE);
//    while (d == 1)
//    {
//        printf("---%dline---\n", i);
//        d = get_next_line(fd, &line);
//        printf("%s\t", line);
//        free(line);
//        printf("d : %d\n", d);
//        i++;
//    }
//    close(fd);
//    // system("leaks a.out");
//    return (0);
//}
