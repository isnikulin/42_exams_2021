#include "get_next_line.h"

char *ft_strjoin(char *line, char c)
{
	char *tmp;
	int i = 0, j = 0;
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
	char *tmp = NULL;
	char c = 0;
	if (tmp == NULL)
		tmp = ft_strjoin(tmp, '\0');
	while (read(0, &c, 1) && c != '\n')
	{
		tmp = ft_strjoin(tmp, c);
	}
	*line = tmp;
	return (c=='\n');
}
