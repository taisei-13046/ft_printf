#include "get_next_line.h"

#define BUFFER_SIZE 100

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

char	*split_save_after(char *save)
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
	save = ft_substr(&save[len], 1, ft_strlen(save));
	return (save);
}

//save を改行前と改行後に分ける
char	*split_save(char *save)
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
	return (tmp);
}

int	get_next_line(int fd, char **line)
{
	int			flag;
	ssize_t		rd_cnt;
	char		buf[BUFFER_SIZE + 1];
	static char	*save;
	char		*tmp;
	int			check_flag;

	check_flag = 0;
	flag = 1;
	//fd,line,BUFFER_SIZEが正しくなければreturn(-1)
	if (fd < 0 || !line || BUFFER_SIZE < 0)
		return (-1);
	//saveに情報を入れたい　save + buf
    	//readがEOFでない(flagが１)　＆＆　saveに改行がない
	while (flag == 1 && !ft_strchr(save, '\n') && check_flag == 0)
	{
       		//read(bufに)する
		if ((rd_cnt = read(fd, buf, BUFFER_SIZE)) < 0)
            	free(buf);//できなかった時free
		buf[rd_cnt] = 0;
		if (rd_cnt == 0)
		{
			flag = 0;
			break ;
		}
        	//saveに過去のsaveとbufを足したものを代入 save = save + buf (strjoin)
		save = ft_strjoin(save, buf);
        	//(改行があったら)bufをフリーして、saveの内容をlineに移す。 line = save
		tmp = malloc (sizeof(char) * 100);
		tmp = split_save(save);
		if (save != tmp)
			check_flag = 1;
		save = split_save_after(save);
		//if (*line)
		//	free(*line);
		*line = tmp;
	}
	//saveに改行がる　→　改行までコピー
	return (flag);
}

#include <stdio.h>
int	main(void)
{
	char	*line;
	int		fd;

	fd = open("test.txt", O_RDONLY);
	if (fd == -1)
		return (0);
	while (get_next_line(fd, &line) > 0)
		printf("%s\n", line);
	return (0);
}
