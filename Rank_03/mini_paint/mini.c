#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define ERR_ARG "Error: argument"
#define ERR_OP "Error: Operation file corrupted"

typedef struct s_zone
{
	int w;
	int h;
	char bg;
}				t_zone;

typedef struct s_shape
{
	char type;
	float x;
	float y;
	float r;
	char c;
}			t_shape;

int ft_strlen(char *s)
{
	int i = 0;
	if (s)
	{
		while (s[i])
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

int ft_check_zone_elem(int a)
{
	return (a <= 300 && a >= 0);
}

char *ft_paint_canvas(FILE *file, t_zone *bg)
{
	char *ret = NULL;
	int count = fscanf(file, "%d %d %c\n",  &bg->w, &bg->h, &bg->bg);
	if (count != 3)
		ret = NULL;
	else if (ft_check_zone_elem(bg->w) && ft_check_zone_elem(bg->h))
	{
		ret = (char *)calloc(sizeof(char), bg->w * bg->h + 1);
		ret = memset(ret, bg->bg, bg->w * bg->h);
	}
	return (ret);
}

int is_in_shape(float x, float y, t_shape *tmp)
{
	float dist = sqrtf(powf(x - tmp->x, 2.00000000) + powf(y - tmp->y, 2.00000000));
	float e = 1.00000000;

	if (dist <= tmp->r)
	{
		if(tmp->r - dist < e)
			return (2);
		return (1);
	}
	return (0);
}

void ft_put_shape(t_shape *tmp, t_zone *bg, char **canvas)
{
	int x, y, flag;

	y = -1;
	while (++y < bg->h)
	{
		x = -1;
		while (++x < bg->w)
		{
			flag = is_in_shape(x, y, tmp);
			if ((tmp->type == 'c' && flag == 2) || (tmp->type == 'C' && flag))
				(*canvas)[y * bg->w + x] = tmp->c;
		}
	}
}

int ft_paint_shapes(FILE *file, t_zone *bg, char **canvas)
{
	int count = 0;
	t_shape tmp = {0};

	while ((count = fscanf(file, "%c %f %f %f %c\n", &tmp.type, &tmp.x, &tmp.y, &tmp.r, &tmp.c)) == 5)	{
		if (tmp.r <= 0.00000000 && (tmp.type != 'c' || tmp.type != 'C'))
			return (0);
		ft_put_shape(&tmp, bg, canvas);
	}
	if (count != -1)
		return 0;
	return 1;
}

void ft_draw(char *canvas, t_zone *bg)
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
	t_zone bg = {0};
	char *canvas = NULL;
	FILE *file = NULL;

	if (argc != 2)	{
		ret = ft_putstr(ERR_ARG, 1);
	}	else if (!(file = fopen(argv[1], "r")))	{
		ret = ft_putstr(ERR_OP, 1);
	}	else if (!(canvas = ft_paint_canvas(file, &bg)))	{
		ret = ft_putstr(ERR_OP, 1);
	}	else if (!ft_paint_shapes(file, &bg, &canvas))	{
		ret = ft_putstr(ERR_OP, 1);
	} else	{
		ft_draw(canvas, &bg);
	}
	if (canvas)
		free(canvas);
	if (file)
		fclose(file);
	return (ret);
}
