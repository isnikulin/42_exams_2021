#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#define ERR_ARG "Error: argument\n"
#define ERR_OPF "Error: Operation file corrupted\n"

#define SCAN_CONF "%d %d %c\n"
#define SCAN_SHAPE "%c %f %f %f %c\n"

typedef struct s_config{
	int w;
	int h;
	char bg;
}	t_config;

typedef struct s_shape{
	char type;
	float x;
	float y;
	float r;
	char c;
}	t_shape;

static int ft_strlen(char *s)
{
	int i= 0;
	while (s[i])
		i++;
	return (i);
}

static void ft_putstr(char *s)
{
	if (s)
		write(1, s, ft_strlen(s));
}

static bool is_conf(int a)
{
	return (a > 0 && a <= 300);
}

static char *make_canvas(FILE *file, t_config *config) {
	char *ret = NULL;
	int count = fscanf(file, SCAN_CONF, &config->w, &config->h, &config->bg);

	if (count == 3 && is_conf(config->w) && is_conf(config->h)) {
		ret = (char*)calloc(config->w * config->h + 1, 1);
		ret = memset(ret, config->bg, config->w * config->h);
	}
	return (ret);
}

static int is_in_shape(float x, float y, t_shape *tmp)
{
	float e = 1.00000000;
	float dist = sqrtf(powf(x - tmp->x, 2.00000000) + powf(y - tmp->y, 2.00000000));

	if (dist <= tmp->r) {
		if (tmp->r - dist < e)
			return (2);
		return (1);
	}
	return (0);
}

static void put_shape_on_canvas(t_config *config, char *buf, t_shape *tmp)
{
	for (int y = 0; y < config->h; y++) {
		for(int x = 0; x < config->w; x++) {
			int flag = is_in_shape((float) x, (float) y, tmp);
			if ((flag == 2 && tmp->type == 'c') || (flag && tmp->type == 'C')) {
				buf[y * config->w + x] = tmp->c;
			}
		}
	}
}

static bool do_shapes(FILE *file, t_config *config, char *buf) {
	t_shape tmp = {0};
	int count = 0;
	while((count = fscanf(file, SCAN_SHAPE, &tmp.type, &tmp.x, &tmp.y, &tmp.r, &tmp.c)) == 5) {
		if (tmp.r > 0.00000000 && (tmp.type == 'c' || tmp.type == 'C'))		{			
			put_shape_on_canvas(config, buf, &tmp);
			//shape on canvas
		} else {
			return (false);
		}
	}
	if (count != -1)
		return (false);
	return (true);
}

static void paint(char *canvas, t_config *config){
	for(int i = 0; i<config->h; i++) {
		write(1, canvas + (i * config->w), config->w);
		write(1, "\n", 1);
	}
}

int main(int argc, char *argv[])
{
	int ret = 1;
	FILE *file = NULL;
	char *canvas = NULL;
	t_config config = {0};

	if (argc != 2)	{
		ft_putstr(ERR_ARG);
	} else if (!(file = fopen(argv[1], "r"))) {
		ft_putstr(ERR_OPF);
	} else if (!(canvas = make_canvas(file, &config))) {
		ft_putstr(ERR_OPF);
	} else if (!do_shapes(file, &config, canvas)){
		ft_putstr(ERR_OPF);
	} else {
		paint(canvas, &config);
		ret = 0;
	}


	if (file) {
		fclose(file);
	}
	if (canvas) {
		free(canvas);
	}
	return (ret);
}
