#include "get_next_line.h"

/*expand line to + 1 char and put char in it*/
char *ft_strjoin(char *line, char c)
{
	int i = 0, j = 0;
	char *tmp;
	if (!line)
	{
		tmp = (char*)malloc(2);
		tmp[0] = c;
		tmp[1] = 0;
	}
	else
	{
		while(line[i])
			i++;
		tmp = (char*)malloc(i + 2);
		while(j < i)
		{
			tmp[j] = line[j];
			j++;
		}
		tmp[j] = c;
		tmp[j+1] = 0;
		free(line);
	}
	return (tmp);
}

int get_next_line(char **line)
{
	char c = 0;
	char *tmp = NULL;
	while(read(0, &c, 1) && c != '\n')
	{
		tmp = ft_strjoin(tmp, c);
	}
	if (tmp == NULL)
		tmp = ft_strjoin(tmp, '\0');
	*line = tmp;
	return (c=='\n');
}
