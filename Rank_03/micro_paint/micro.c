#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define ERR_ARG "Error: argument"
#define ERR_OP "Error: Operation file corrupted"


typedef struct	s_zone
{
	int w;
	int h;
	char bg;
}				t_zone;

typedef struct	s_shape
{
	char type;
	float x;
	float y;
	float w;
	float h;
	char c;
}				t_shape;

int ft_strlen(char *s)
{
	int i = 0;
	if (s)
	{
		while(s[i])
			i++;
	}
	return (i);
}

int ft_putstr(char *s, int ret)
{
	if (s)
	{
		write(1, s, ft_strlen(s));
		write(1, "\n", 1);
	}
	return (ret);
}

int	ft_valid_zone_elem(int a)
{
	return(a >= 0 && a <= 300);
}

char *ft_make_canvas(FILE *file, t_zone *bg)
{
	char *ret = NULL;
	int count = fscanf(file, "%d %d %c\n", &bg->w, &bg->h, &bg->bg);

	if (count == 3 && ft_valid_zone_elem(bg->w) && ft_valid_zone_elem(bg->h))
	{
		ret = (char *)calloc(sizeof(char), bg->w * bg->h + 1);
		ret = memset(ret, bg->bg, bg->w * bg->h);
	}
	return (ret);
}

int	ft_shape_in_zone(int y, int x, t_shape *tmp)
{
	float e = 1.00000000;
	if (x < tmp->x || (tmp->x + tmp->w) < x || y < tmp->y || tmp->y + tmp->h < y)
		return (0);
	if (x - tmp->x < e || tmp->x + tmp->w - x < e || y - tmp->y < e || tmp->y + tmp->h - y < e)
		return (2);
	return (1);
}

void	ft_put_shape(char **canvas, t_shape *tmp, t_zone *bg)
{
	int x = -1, y = -1, flag = 0;
	
	while(++y < bg->h)
	{
		x = -1;
		while(++x < bg->w)
		{
			flag = ft_shape_in_zone(y, x, tmp);
			if ((tmp->type == 'r' && flag == 2) || (tmp->type == 'R' && flag))
				(*canvas)[y * bg->w + x] = tmp->c;
		}
	}
}

int	ft_is_tmp_valid(t_shape *tmp)
{
	return (tmp->w > 0.00000000 && tmp->h > 0.00000000 && (tmp->type == 'r' || tmp->type == 'R' ));
}

int ft_paint_shapes(char **canvas, FILE *file, t_zone *bg)
{
	t_shape tmp = {0};
	int	count = 0;

	while ((count = fscanf(file, "%c %f %f %f %f %c\n", &tmp.type, &tmp.x, &tmp.y, &tmp.w, &tmp.h, &tmp.c)) == 6)	{
		if (!ft_is_tmp_valid(&tmp))
			return (0);
		ft_put_shape(canvas, &tmp, bg);
	}
	if (count != -1)
		return (0);
	return (1);
}

void ft_paint(char *canvas, t_zone *bg)
{
	int i = -1;
	while (++i < bg->h)
	{
		write(1, canvas + (i * bg->w), bg->w);
		write(1, "\n", 1);
	}
}

int main(int argc, char *argv[])
{
	int ret = 0;
	FILE *file = NULL;
	char *canvas = NULL;
	t_zone bg = {0};

	if (argc != 2)	{
		ret = ft_putstr(ERR_ARG, 1);
	}	else if (!(file = fopen(argv[1], "r")))	{
		ret = ft_putstr(ERR_OP, 1);
	}	else if (!(canvas = ft_make_canvas(file, &bg)))	{
		ret = ft_putstr(ERR_OP, 1);
	}	else if (!ft_paint_shapes(&canvas, file, &bg))	{
		ret = ft_putstr(ERR_OP, 1);
	}	else	{
		ft_paint(canvas, &bg);
	}
	if (file)
		fclose(file);
	if (canvas)
		free(canvas);
	return (ret);
}
