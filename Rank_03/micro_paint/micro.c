#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARG_ERR "Error: argument"
#define F_ERR "Error: Operation file corrupted"

typedef struct  s_zone
{
    int w;
    int h;
    char bg;
}               t_zone;

typedef struct  s_rec
{
    char type;
    float x;
    float y;
    float w;
    float h;
    char color;
}               t_rec;

int ft_strlen(char *str)
{
    if (!str)
        return (0);
	int i = -1;
	while (str[++i]);
	return (i);
}

int ft_putstr(char *str, int ret)
{
    write(1, str, ft_strlen(str));
    write(1, "\n", 1);
    return (ret);
}

char *ft_check_draw_zone(FILE *fd, t_zone *zone)
{
    int count = fscanf(fd, "%d %d %c\n", &zone->w, &zone->h, &zone->bg);
    if (count != 3 ) 
        return (NULL);
    else if (!(zone->w >= 0 && zone->w <= 300 && zone->h >= 0 && zone->h <= 300))
        return (NULL);
    char *ret = (char *)calloc(1, zone->h * zone->w + 1);
    return (memset(ret, zone->bg, zone->h * zone->w));
}

//printf("%c %f %f %f %f %c\n", line->type, line->x, line->y, line->w, line->h, line->color); 
int read_config(FILE *fd, t_rec *line)
{
    int count = 0;
    while ((count = fscanf(fd, "%c %f %f %f %f %c\n", &line->type, &line->x, &line->y, &line->w, 
              &line->h, &line->color)) == 6)
    {
            ;    //code for muting characters
    }
    if (count !=  (-1))
        return (0);
    return (1);
}

void print_draw(char *canvas, t_zone *zone)
{
    int i = 0;

    while (i < zone->h)
    {
        write(1, canvas + (i * zone->w), zone->w);
        write(1, "\n", 1);
        i++;
    }
}


int main(int argc, char *argv[])
{
    FILE *fd = NULL;
    t_zone zone = {0};
    char *canvas = NULL;
    int exit_code = 0;
    t_rec line = {0};

	if (argc != 2)  {
		return(ft_putstr(ARG_ERR, 1));
    }   else  if (!(fd = fopen(argv[1], "r")))  {
        return(ft_putstr(F_ERR, 1)); 
    }   else if (!(canvas = ft_check_draw_zone(fd, &zone))) {
        exit_code = ft_putstr(F_ERR, 1); 
    }   else if (!read_config(fd, &line)) {
        exit_code = ft_putstr(F_ERR, 1); 
    }
    if (canvas) {
        print_draw(canvas, &zone);
        free(canvas);
    }
    fclose(fd);
	return (exit_code);
}
