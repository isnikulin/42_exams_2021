#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERR_ARG "Error: argument"
#define ERR_OPFILE "Error: Operation file corrupted"

typedef struct s_canvas
{
	int	w;
	int h;
	char bg;
}				t_canvas;

typedef struct shape
{
	char type;
	float x;
	float y;
	float w;
	float h;
	char color;
}				t_shape;

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
	write(1, s, ft_strlen(s));
	write(1, "\n", 1);
	return (ret);
}

int is_wh_correct(int a)
{
	return (a >= 0 && a<= 300);
}

char *ft_make_canvas(t_canvas *canvas, FILE *file)
{
	int count = fscanf(file, "%d %d %c\n", &canvas->w, &canvas->h, &canvas->bg);
	char *ret = NULL;
	if (count == 3 && is_wh_correct(canvas->w) && is_wh_correct(canvas->h))
		ret = (char *)calloc(sizeof(char), canvas->w * canvas->h + 1);
	return (memset(ret, canvas->bg, canvas->w * canvas->h));
}

int ft_check_shape(t_shape *shape)
{
	return (shape->w > 0.00000000 && shape->h > 0.00000000 
			&& (shape->type == 'r' || shape->type == 'R'));
}

int is_in_rectangle(float x, float y, t_shape *shape)
{
	float c = 1.00000000;
	if ((x < shape->x) || (shape->x + shape->w < x) ||
		 (y < shape->y) || (shape->y + shape->h < y))
		return (0);
	if ((x - shape->x < c) || ((shape->x + shape->w) - x < c) ||
		(y - shape->y < c) || ((shape->y + shape->h) - y < c))
		return (2);
	return (1);
}

void ft_draw_shape(char **canvas, t_shape *shape, t_canvas *zone)
{
	int i = -1;
	int j = 0; 
	int is_drawable = 0;

	while (++i < zone->h)
	{
		j = -1;
		while(++j < zone->w)
		{
			is_drawable = is_in_rectangle(j, i, shape);
			if ((shape->type == 'r' && is_drawable == 2) ||
				(shape->type == 'R' && is_drawable))
				(*canvas)[i * zone->w + j] = shape->color;
		}
	}
}

int	ft_paint_shapes(FILE *file, char **canvas, t_canvas *zone)
{
	t_shape tmp = {0};
	int count  ;
	while ((count = fscanf(file, "%c %f %f %f %f %c\n", &tmp.type, &tmp.x, &tmp.y, &tmp.w, &tmp.h, &tmp.color)) == 6)
	{
		if (!ft_check_shape(&tmp))
			return (0);
		ft_draw_shape(canvas, &tmp, zone);
	}
	if (count != -1)
		return (0);
	return (1);
}

void ft_paint(char *canvas, t_canvas *zone)
{
	int i = -1;
	while (++i < zone->h)
	{
		write(1, canvas + (i * zone->w), zone->w);
		write(1, "\n", 1);
	}
}

int main(int argc, char *argv[])
{
	t_canvas zone = {0};
	int ret = 0;
	FILE *file = NULL;
	char *canvas = NULL;

	if (argc != 2)
		ret = ft_putstr(ERR_ARG, 1);
	else if (!(file = fopen(argv[1], "r"))) {
		ret = ft_putstr(ERR_OPFILE, 1);
	} else if (!(canvas = ft_make_canvas(&zone, file))) {
		ret = ft_putstr(ERR_OPFILE, 1);
	} else if (!(ft_paint_shapes(file, &canvas, &zone)))
		ret = ft_putstr(ERR_OPFILE, 1);
	ft_paint(canvas, &zone);
	if (file)
		fclose(file);
	if (canvas)
		free(canvas);
	return (ret);
}

