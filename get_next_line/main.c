#include "get_next_line.h"
#include <stdio.h>

int	main(int argc, char **argv)
{
	char	*list;
	int		fd;

	fd = open("test.txt", O_RDONLY);
	if (fd == -1)
		return (0);
	while (get_next_line(fd, &list) > 0)
		printf("%s\n", list);
	return (0);
}
